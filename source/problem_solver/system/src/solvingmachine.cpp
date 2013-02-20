/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "solvingmachine.h"
#include "datalayerread.h"
#include "utils.h"

#include <boost/foreach.hpp>
#include <boost/format.hpp>

namespace ProblemSolver
{

SolvingMachine::SolvingMachine(IDataLayerRead& dataLayer):
    _dataLayer(dataLayer)
{
}

/**
 * Returns a suggested continue path for identifying the input unknown problem.
 */
SolvingMachine::Suggestion SolvingMachine::makeSuggestion(const Investigation& investigation)
{
    Suggestion suggestion;
    
    // look up the standard things
    SymptomMap positiveSymptoms;
    _dataLayer.get(investigation.positiveSymptoms, positiveSymptoms);
    
    SymptomMap negativeSymptoms;
    _dataLayer.get(investigation.negativeSymptoms, negativeSymptoms);
    
    ProblemMap negativeProblems;
    _dataLayer.get(investigation.negativeProblems, negativeProblems);
    
    SolutionMap negativeSolutions;
    _dataLayer.get(investigation.negativeSolutions, negativeSolutions);
    
    CategoryBranch partialCategoryBranch;
    
    // generate partial category branch
    BOOST_FOREACH(const SymptomMap::value_type& pair, positiveSymptoms)
    {
        partialCategoryBranch.insert(pair.second.categoryID);
    }
    
    BOOST_FOREACH(const SymptomMap::value_type& pair, negativeSymptoms)
    {
        partialCategoryBranch.insert(pair.second.categoryID);
    }
    
    BOOST_FOREACH(const ProblemMap::value_type& pair, negativeProblems)
    {
        partialCategoryBranch.insert(pair.second.categoryID);
    }
    
    BOOST_FOREACH(const SolutionMap::value_type& pair, negativeSolutions)
    {
        partialCategoryBranch.insert(pair.second.categoryID);
    }
    
    Problem positiveProblem;
    
    if(!investigation.positiveProblem.empty())
    {
        // look up the positive problem
        ProblemMap positiveProblems;
        std::vector<Identifier> positiveID;
        positiveID.push_back(investigation.positiveProblem);
        
        _dataLayer.get(positiveID, positiveProblems);
        positiveProblem = positiveProblems.begin()->second;
        
        partialCategoryBranch.insert(positiveProblem.categoryID);
    }
    
    Solution positiveSolution;
    
    if(!investigation.positiveSolution.empty())
    {
        // look up the positive solution
        SolutionMap positiveSolutions;
        std::vector<Identifier> positiveID;
        positiveID.push_back(investigation.positiveSolution);
        
        _dataLayer.get(positiveID, positiveSolutions);
        positiveSolution = positiveSolutions.begin()->second;
        
        partialCategoryBranch.insert(positiveSolution.categoryID);
    }
    
    CategoryBranch fullCategoryBranch = buildCategoryBranch(partialCategoryBranch);
    
    if(!investigation.positiveProblem.empty())
    {
        // handle the case where we have already identified the problem
        SymptomsWithSameProblem relatedSymptoms;
        _dataLayer.getLinksByProblem(investigation.positiveProblem, relatedSymptoms);
        
        SymptomMap relevantSymptoms;
        filterByBranch(relatedSymptoms, fullCategoryBranch, relevantSymptoms);
        
        // suggest checking all related symptoms for statistical purposes only
        BOOST_FOREACH(const SymptomMap::value_type& pair, relevantSymptoms)
        {
            const Symptom& symptom = pair.second;
            
            // check if the symptom is already checked or banned
            if(positiveSymptoms.find(symptom.id) != positiveSymptoms.end() ||
               negativeSymptoms.find(symptom.id) != negativeSymptoms.end() ||
               utils::contains(investigation.bannedSymptoms, symptom.id))
            {
                continue;
            }
            
            // add the symptom for verification
            suggestion.symptoms.push_back(symptom.id);
            suggestion.symptomValues.push_back(0); // static 0 as these are just for statistics!
        }
        
        if(investigation.positiveSolution.empty())
        {
            // in case we have no working solution yet, suggest new ones
            SolutionsWithSameProblem relatedSolutions;
            _dataLayer.getLinksByProblem(investigation.positiveProblem, relatedSolutions);

            SolutionMap relevantSolutions;
            filterByBranch(relatedSolutions, fullCategoryBranch, relevantSolutions);
            
            BOOST_FOREACH(const SolutionMap::value_type& pair, relevantSolutions)
            {
                const Solution& solution = pair.second;
                
                // check if the solution is already checked or banned
                if(negativeSolutions.find(solution.id) != negativeSolutions.end() ||
                   utils::contains(investigation.bannedSolutions, solution.id))
                {
                    continue;
                }
                
                SolutionLink& link = relatedSolutions[solution.id];
                
                // add the solution for verification
                suggestion.solutions.push_back(solution.id);
                suggestion.solutionValues.push_back(calculateValue(solution, link));
            }
        }
        
    }
    else if(!investigation.positiveSolution.empty())
    {
        // in case we have no working solution yet, suggest new ones
        ProblemsWithSameSolution relatedProblems;
        _dataLayer.getLinksBySolution(investigation.positiveSolution, relatedProblems);

        ProblemMap relevantProblems;
        filterByBranch(relatedProblems, fullCategoryBranch, relevantProblems);
        
        BOOST_FOREACH(const ProblemMap::value_type& pair, relevantProblems)
        {
            const Problem& problem = pair.second;
            
            // check if the problem is already checked or banned
            if(negativeProblems.find(problem.id) != negativeProblems.end() ||
               utils::contains(investigation.bannedProblems, problem.id))
            {
                continue;
            }
            
            SolutionLink& link = relatedProblems[problem.id];
            
            // add the solution for verification
            suggestion.problems.push_back(problem.id);
            suggestion.problemValues.push_back(calculateValue(problem, link));
        }
    }
    else
    {
        // we don't know the problem yet, so we must make a more complex suggestion
        
        // we will load all information that we need
        
        // these are the problems we are considering for suggestion
        ProblemMap subjectProblems;
        
        // these are the symptoms we are considering for suggestion
        SymptomMap subjectSymptoms;
        
        // this will hold all symptom-to-problem links by symptom ID
        SymptomToLinks allSymptomLinks;
        
        // this will hold all problem-to-symptom links by problem ID
        ProblemToLinks allProblemLinks;
        
        // this will be used only while retrieving any objects
        boost::unordered_set<Identifier> aggregationOfObjects;
        std::vector<Identifier> objectsToBeLoaded;
        /** \todo Lubo: Everywhere where there are connections check if positive != 0 !!! (and in the above) */
        // retrieve symptoms links and aggregate related problems
        BOOST_FOREACH(const SymptomMap::value_type& pair, positiveSymptoms)
        {
            ProblemsWithSameSymptom& relatedProblems = allSymptomLinks[pair.second.id];
            _dataLayer.getLinksBySymptom(pair.second.id, relatedProblems);
            
            // aggregate all related problems
            BOOST_FOREACH(const ProblemsWithSameSymptom::value_type& pair, relatedProblems)
            {
                aggregationOfObjects.insert(pair.second.problemID);
            }
        }
        
        
        // retrieve the subject problems
        BOOST_FOREACH(CIdentifier problemID, aggregationOfObjects)
        {
            // check if the problem is already checked or banned
            if(negativeProblems.find(problemID) != negativeProblems.end() ||
               utils::contains(investigation.bannedProblems, problemID))
            {
                continue;
            }
            
            objectsToBeLoaded.push_back(problemID);
        }
        
        /** \todo Lubo: ALL OF THESE SHOULD FILTER BY HAVING AT LEAST 1 POSITIVE (after denominating) */
        _dataLayer.get(objectsToBeLoaded, subjectProblems);
        filterByBranch(fullCategoryBranch, subjectProblems);
        
        // clear the buffers
        aggregationOfObjects.clear();
        objectsToBeLoaded.clear();

        
        // retrieve problem links and aggregate related symptoms
        BOOST_FOREACH(const ProblemMap::value_type& pair, subjectProblems)
        {
            SymptomsWithSameProblem& relatedSymptoms = allProblemLinks[pair.second.id];
            _dataLayer.getLinksByProblem(pair.second.id, relatedSymptoms);
            
            // aggregate all related symptoms
            BOOST_FOREACH(const SymptomsWithSameProblem::value_type& pair, relatedSymptoms)
            {
                aggregationOfObjects.insert(pair.second.symptomID);
            }
        }
        
        
        // retrieve the subject symptoms
        BOOST_FOREACH(CIdentifier symptomID, aggregationOfObjects)
        {
            // check if the symptom is already checked or banned
            if(positiveSymptoms.find(symptomID) != positiveSymptoms.end() ||
               negativeSymptoms.find(symptomID) != negativeSymptoms.end() ||
               utils::contains(investigation.bannedSymptoms, symptomID))
            {
                continue;
            }
            
            objectsToBeLoaded.push_back(symptomID);
        }
        
        _dataLayer.get(objectsToBeLoaded, subjectSymptoms);
        filterByBranch(fullCategoryBranch, subjectSymptoms);
        
        // clear the buffers
        aggregationOfObjects.clear();
        objectsToBeLoaded.clear();
        
        if(!subjectProblems.empty())
        {
            // add problems to the suggestion
            BOOST_FOREACH(const ProblemMap::value_type& pair, subjectProblems)
            {
                int problemValue = calculateValue(pair.second, allProblemLinks[pair.second.id], positiveSymptoms);
                suggestion.problems.push_back(pair.second.id);
                suggestion.problemValues.push_back(problemValue);
            }
            
            // add symptoms to the suggestion
            BOOST_FOREACH(const SymptomMap::value_type& pair, subjectSymptoms)
            {
                int symptomValue = calculateValue(pair.second, subjectProblems, allProblemLinks, positiveSymptoms, suggestion);
                suggestion.symptoms.push_back(pair.second.id);
                suggestion.symptomValues.push_back(symptomValue);
            }

            /** \todo Lubo: This could also suggest solutions without having a positive problem */
        }
    }
    
    return suggestion;
}

/**
 * Builds a full category branch based on partial categories from the branch
 */
SolvingMachine::CategoryBranch SolvingMachine::buildCategoryBranch(CategoryBranch partialBranch)
{
    CategoryMap allCategories;
    _dataLayer.get(std::vector<Identifier>(), allCategories);

    // generate "root" branch for each category and check if all categories in the partialBranch are in it
    BOOST_FOREACH(CIdentifier categoryID, partialBranch)
    {
        CategoryBranch rootBranch;
        Identifier parentCategory = categoryID;
        
        while(!parentCategory.empty())
        {
            CategoryMap::iterator iterator = allCategories.find(parentCategory);
            if(iterator == allCategories.end())
            {
                throw Exception((boost::format("SolvingMachine: Cannot find %s category.") % parentCategory).str());
            }

            rootBranch.insert(iterator->second.id);
            parentCategory = iterator->second.parent;
        }
        
        bool isLeafBranch = true;
        BOOST_FOREACH(CIdentifier partialBranchCategoryID, partialBranch)
        {
            if(rootBranch.find(partialBranchCategoryID) == rootBranch.end())
            {
                isLeafBranch = false;
                break;
            }
        }
        
        if(isLeafBranch)
        {
            // add the child categories to the root branch
            addChilds(categoryID, rootBranch, allCategories);
            
            // return the now ready full category branch
            return rootBranch;
        }
    }

    throw Exception("SolvingMachine: Cannot determine working category branch.");
}

/**
 * Recursively add all child categories on a given category
 */
void SolvingMachine::addChilds(CIdentifier categoryID, CategoryBranch& result, const CategoryMap& allCategories)
{
    CategoryMap::const_iterator iterator = allCategories.find(categoryID);
    if(iterator == allCategories.end())
    {
        throw Exception((boost::format("SolvingMachine: Cannot find %s category.") % categoryID).str());
    }
    
    // add self
    result.insert(categoryID);
    
    // add children
    BOOST_FOREACH(CIdentifier childCategoryID, iterator->second.childs)
    {
        addChilds(childCategoryID, result, allCategories);
    }
}

/**
 * Loads all objects pointed to by the links and filters them by the input branch
 */
template< class Links, class Objects>
void SolvingMachine::filterByBranch(const Links& relatedLinks, const CategoryBranch& categoryBranch, Objects& result)
{
    std::vector<Identifier> objectIDs;
    BOOST_FOREACH(const typename Links::value_type& pair, relatedLinks)
    {
        objectIDs.push_back(pair.first);
    }
    
    _dataLayer.get(objectIDs, result);
    
    filterByBranch(categoryBranch, result);
}

/**
 * Filters the objects inside result using the supplied categoryBranch
 */
template<class Objects>
void SolvingMachine::filterByBranch(const CategoryBranch& categoryBranch, Objects& result)
{
    typename Objects::const_iterator it = result.begin();
    while(it != result.end())
    {
        if(categoryBranch.find(it->second.categoryID) == categoryBranch.end())
        {
            // delete irrelevant objects
            it = result.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

/**
 * Calculates the penalty that should be applied based on the difficulty level
 */
double SolvingMachine::getDifficultyPenalty(DifficultyLevel level)
{
    // Penalty for gurus:
    /*
    static double guruPenalties[] =
    {
        1,      // difficultyUnknown
        1,      // difficultyOneLook
        0.9875, // difficultyJustByIntuition
        0.975,  // difficultyJustByLogic
        0.95,   // difficultySomeExplanation
        0.925,  // difficultyModerateExplanation
        0.900,  // difficultyLotsOfExplanation
        0.875,  // difficultyExplanationMightNotHelp
        0.825,  // difficultyNeedToHaveDoneIt
        0.775,  // difficultyManyTimesAndStillNotEasy
        0.725   // difficultyCategoryExpertOnly
    };*/

    // Penalty for experts:
    static double expertPenalties[] =
    {
        1,      // difficultyUnknown
        1,      // difficultyOneLook
        0.975, // difficultyJustByIntuition
        0.95,  // difficultyJustByLogic
        0.90,   // difficultySomeExplanation
        0.85,  // difficultyModerateExplanation
        0.80,  // difficultyLotsOfExplanation
        0.75,  // difficultyExplanationMightNotHelp
        0.65,  // difficultyNeedToHaveDoneIt
        0.55,  // difficultyManyTimesAndStillNotEasy
        0.45   // difficultyCategoryExpertOnly
    };
    
    /*
    // Penalty for rookies:
    static double rookiePenalties[] =
    {
        1,      // difficultyUnknown
        1,      // difficultyOneLook
        0.95, // difficultyJustByIntuition
        0.90,  // difficultyJustByLogic
        0.80,   // difficultySomeExplanation
        0.70,  // difficultyModerateExplanation
        0.60,  // difficultyLotsOfExplanation
        0.50,  // difficultyExplanationMightNotHelp
        0.35,  // difficultyNeedToHaveDoneIt
        0.20,  // difficultyManyTimesAndStillNotEasy
        0.05   // difficultyCategoryExpertOnly
    };*/

    /** \todo Lubo: Users should send their skill level with requests in order to make better suggestions */
    return expertPenalties[level];
}

/**
 * Calculates a number between 0 and 100 that represents the accuracy of the references
 */
double SolvingMachine::calculateAccuracity(int firstReferences, int secondReferences)
{
    double greatest = std::max(firstReferences, secondReferences);
    if(greatest < MAX_REFERENCES)
        return 100.*(greatest / MAX_REFERENCES);
    
    return 100.;
}

/**
 * Calculates a number between 0 and 1 that should be applied to references to be normalized
 */
double SolvingMachine::calculateReduction(int firstReferences, int secondReferences)
{
    double greatest = std::max(firstReferences, secondReferences);
    if(greatest > MAX_REFERENCES)
        return MAX_REFERENCES/greatest;
    
    return 0.;
}

/**
 * Calculates a value between 0 and 100 representing the connection between the positive and negative references.
 */
double SolvingMachine::calculateValue(int positiveReferences, int negativeReferences)
{
    double accuracy = calculateAccuracity(positiveReferences, negativeReferences);
    double reduction = calculateReduction(positiveReferences, negativeReferences);
    
    double positive = positiveReferences*reduction;
    double negative = negativeReferences*reduction;
    double value = accuracy * (positive / (positive + negative));
    
    return value;
}

/**
 * Calculates the value of a solution-problem link
 */
int SolvingMachine::calculateValue(const GenericInfo& object, const SolutionLink& link)
{
    double value = calculateValue(link.positive, link.negative);
    value *= getDifficultyPenalty(object.difficulty);
    
    if(!link.confirmed)
        value *= UNCONFIRMED_PENALTY; // apply penalty for non-confirmed links
    
    if(!object.confirmed)
        value *= UNCONFIRMED_PENALTY; // apply penalty for non-confirmed solution
        
    return value;
}

/**
 * Calculates the value of a subject symptom.
 * This is done by re evaluating the subject problems as if the symptom was active.
 * In case the upper bound of problems is smaller than the initial one, then this symptom brings value.
 * The value is the chance of this problem being active.
 */
int SolvingMachine::calculateValue(const Symptom& symptom, const ProblemMap& subjectProblems, const ProblemToLinks& allProblemLinks,
                                   const SymptomMap& positiveSymptoms, const Suggestion& suggestion)
{
    /** \todo Lubo: this should also take in account negative symptoms and problems!!! */
    
    /** \todo Lubo: OPTIMIZE symptom value, it has too much excessive work done */
    SymptomMap theoreticalSymptoms = positiveSymptoms;
    theoreticalSymptoms[symptom.id] = symptom;
    
    std::vector<int> originalUpperProblems;
    std::vector<double> originalProblemChances;
    int originalHighestProblemValue = 0;
    double originalUpperDifficultyPenalty = 0;
    
    // find the original highest problem value
    for(uint i = 0; i < suggestion.problems.size(); ++i)
    {
        if(originalHighestProblemValue < suggestion.problemValues[i])
            originalHighestProblemValue = suggestion.problemValues[i];
        
        const SymptomsWithSameProblem& symptomsWithThisProblem = allProblemLinks.find(suggestion.problems[i])->second;
        SymptomsWithSameProblem::const_iterator it = symptomsWithThisProblem.find(symptom.id);
        
        if(it != symptomsWithThisProblem.end())
        {
            double chanceOfProblemCausingSymptom = calculateValue(it->second.positiveChecks, it->second.negativeChecks);
            if(!it->second.confirmed)
                chanceOfProblemCausingSymptom *= UNCONFIRMED_PENALTY;

            originalProblemChances.push_back(chanceOfProblemCausingSymptom);
        }
        else
            originalProblemChances.push_back(0); // this problem has no chance to cause this symptom
    }
    
    // find the original upper problems
    for(uint i = 0; i < suggestion.problems.size(); ++i)
    {
        if(suggestion.problemValues[i] < (originalHighestProblemValue - UPPER_BOUND_PROBLEM_RANGE))
            continue;
        
        originalUpperProblems.push_back(i);
        originalUpperDifficultyPenalty += (1 - getDifficultyPenalty(subjectProblems.find(suggestion.problems[i])->second.difficulty));
    }

    std::vector<Identifier> newProblems;
    std::vector<int> newProblemValues;
    
    std::vector<int> newUpperProblems;
    int newHighestProblemValue = 0;
    double newUpperDifficultyPenalty = 0;
    
    // find all new values and the highest value
    BOOST_FOREACH(const ProblemMap::value_type& pair, subjectProblems)
    {
        int problemValue = calculateValue(pair.second, allProblemLinks.find(pair.second.id)->second, theoreticalSymptoms);
        newProblems.push_back(pair.second.id);
        newProblemValues.push_back(problemValue);
        
        if(newHighestProblemValue < problemValue)
            newHighestProblemValue = problemValue;
    }

    // find the new upper problems
    for(uint i = 0; i < newProblems.size(); ++i)
    {
        if(newProblemValues[i] < (newHighestProblemValue - UPPER_BOUND_PROBLEM_RANGE))
            continue;
        
        newUpperProblems.push_back(i);
        newUpperDifficultyPenalty += (1 - getDifficultyPenalty(subjectProblems.find(newProblems[i])->second.difficulty));
    }
    
    double difficultyGain = originalUpperDifficultyPenalty - newUpperDifficultyPenalty;
    double symptomDifficulty = (1 - getDifficultyPenalty(symptom.difficulty));
    int fewerProblems = originalUpperProblems.size() - newUpperProblems.size();
    
    if(difficultyGain < 0 || fewerProblems >= 0)
    {
        return 0; // there is no value in checking this symptom at this time
    }
    
    /** \todo Lubo: this probably needs rework and be based only on the GAIN! */
    // there is value in this symptom, calculate the chance of having it
    double totalValue = 0;
    for(uint i = 0; i < originalUpperProblems.size(); ++i)
    {
        totalValue += originalProblemChances[originalUpperProblems[i]];
    }
    
    double symptomValue = totalValue/originalUpperProblems.size();
    if(difficultyGain > symptomDifficulty)
    {
        // there is definite value in checking this symptom, no need to diminish it by the difficulty
    }
    else
    {
        // there is some value in checking this symptom, diminish it by the difficulty
        /** \todo Lubo: the penalty should be less depending how many symptoms we eliminated */
        symptomValue *= getDifficultyPenalty(symptom.difficulty);
    }
    
    return symptomValue;
}

/**
 * Calculates the value of a subject problem by the probability it has to be positive based on the positive symptoms.
 */
int SolvingMachine::calculateValue(const Problem& problem, const SymptomsWithSameProblem& connectedSymptoms, const SymptomMap& positiveSymptoms)
{
    /** \todo Lubo: this should also take in account negative symptoms and problems!!! */
    int valueOfProblem = 0;
    
    boost::unordered_set<Identifier> missingSymptoms;
    BOOST_FOREACH(const SymptomMap::value_type& pair, positiveSymptoms)
    {
        missingSymptoms.insert(pair.second.id);
    }
    
    double maxHint = 0;
    double totalValue = 0;
    
    int coveredSymptomsCount = 0;
    
    BOOST_FOREACH(const SymptomsWithSameProblem::value_type& pair, connectedSymptoms)
    {
        SymptomMap::const_iterator it = positiveSymptoms.find(pair.second.symptomID);
        if(it == positiveSymptoms.end())
            continue;
        
        ++coveredSymptomsCount;
        missingSymptoms.erase(pair.second.symptomID);
        
        const SymptomLink& link = pair.second;
        
        double chanceOfSymptomHintingProblem = calculateValue(link.positiveChecks, link.falsePositiveChecks);
        double chanceOfProblemCausingSymptom = calculateValue(link.positiveChecks, link.negativeChecks);
        
        if(!it->second.confirmed)
        {
            chanceOfSymptomHintingProblem *= UNCONFIRMED_PENALTY;
            chanceOfProblemCausingSymptom *= UNCONFIRMED_PENALTY;
        }
        
        if(!link.confirmed)
        {
            chanceOfSymptomHintingProblem *= UNCONFIRMED_PENALTY;
            chanceOfProblemCausingSymptom *= UNCONFIRMED_PENALTY;
        }
        
        if(chanceOfSymptomHintingProblem > maxHint)
        {
            maxHint = chanceOfSymptomHintingProblem;
        }
        
        totalValue += std::max(chanceOfSymptomHintingProblem, chanceOfProblemCausingSymptom);
    }

    if(coveredSymptomsCount > 2)
        totalValue /= coveredSymptomsCount;
    else
        totalValue = 1;
    
    valueOfProblem = maxHint*0.5 + totalValue*0.5; // equal weights
    valueOfProblem *= getDifficultyPenalty(problem.difficulty);
    
    BOOST_FOREACH(CIdentifier missingSymptomID, missingSymptoms)
    {
        SymptomMap::const_iterator it = positiveSymptoms.find(missingSymptomID);
        if(it->second.confirmed)
            valueOfProblem *= MISSING_SYMPTOM_PENALTY;
        else
            valueOfProblem *= MISSING_UNCONFIRMED_SYMPTOM_PENALTY;
    }
    
    if(!problem.confirmed)
        valueOfProblem *= UNCONFIRMED_PENALTY;
    
    return valueOfProblem;
}

} // namespace ProblemSolver
