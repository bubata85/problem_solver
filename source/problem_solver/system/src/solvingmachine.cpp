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
    
    if(investigation.positiveProblem != -1)
    {
        // look up the positive problem
        ProblemMap positiveProblems;
        std::vector<int> positiveID;
        positiveID.push_back(investigation.positiveProblem);
        
        _dataLayer.get(positiveID, positiveProblems);
        positiveProblem = positiveProblems.begin()->second;
        
        partialCategoryBranch.insert(positiveProblem.categoryID);
    }
    
    Solution positiveSolution;
    
    if(investigation.positiveSolution != -1)
    {
        // look up the positive solution
        SolutionMap positiveSolutions;
        std::vector<int> positiveID;
        positiveID.push_back(investigation.positiveSolution);
        
        _dataLayer.get(positiveID, positiveSolutions);
        positiveSolution = positiveSolutions.begin()->second;
        
        partialCategoryBranch.insert(positiveSolution.categoryID);
    }
    
    CategoryBranch fullCategoryBranch = buildCategoryBranch(partialCategoryBranch);
    
    if(investigation.positiveProblem != -1)
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
        
        if(investigation.positiveSolution == -1)
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
    else if(investigation.positiveSolution != -1)
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
        /** \todo Lubo: THIS WILL TAKE SOME TIME */
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
        boost::unordered_set<int> aggregationOfObjects;
        std::vector<int> objectsToBeLoaded;
        
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
        BOOST_FOREACH(int problemID, aggregationOfObjects)
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
        BOOST_FOREACH(int symptomID, aggregationOfObjects)
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
        
        
        // add problems to the suggestion
        BOOST_FOREACH(const ProblemMap::value_type& pair, subjectProblems)
        {
            int problemValue = calculateValue(pair.second, allProblemLinks[pair.second.id], positiveSymptoms);
            suggestion.problems.push_back(pair.second.id);
            suggestion.problems.push_back(problemValue);
        }
        
        // add symptoms to the suggestion
        BOOST_FOREACH(const SymptomMap::value_type& pair, subjectSymptoms)
        {
            int symptomValue = calculateValue(pair.second, subjectProblems);
            suggestion.symptoms.push_back(pair.second.id);
            suggestion.symptoms.push_back(symptomValue);
        }

        /** \todo Lubo: This could also suggest solutions without having a positive problem */
    }
    
    return suggestion;
}

/**
 * Builds a full category branch based on partial categories from the branch
 */
SolvingMachine::CategoryBranch SolvingMachine::buildCategoryBranch(CategoryBranch partialBranch)
{
    CategoryMap allCategories;
    _dataLayer.get(std::vector<int>(), allCategories);

    // generate "root" branch for each category and check if all categories in the partialBranch are in it
    BOOST_FOREACH(int categoryID, partialBranch)
    {
        CategoryBranch rootBranch;
        int parentCategory = categoryID;
        
        while(parentCategory != -1)
        {
            CategoryMap::iterator iterator = allCategories.find(parentCategory);
            if(iterator == allCategories.end())
            {
                throw Exception((boost::format("SolvingMachine: Cannot find %d category.") % parentCategory).str());
            }

            rootBranch.insert(iterator->second.id);
            parentCategory = iterator->second.parent;
        }
        
        bool isLeafBranch = true;
        BOOST_FOREACH(int partialBranchCategoryID, partialBranch)
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
void SolvingMachine::addChilds(int categoryID, CategoryBranch& result, const CategoryMap& allCategories)
{
    CategoryMap::const_iterator iterator = allCategories.find(categoryID);
    if(iterator == allCategories.end())
    {
        throw Exception((boost::format("SolvingMachine: Cannot find %d category.") % categoryID).str());
    }
    
    // add self
    result.insert(categoryID);
    
    // add children
    BOOST_FOREACH(int childCategoryID, iterator->second.childCategories)
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
    std::vector<int> objectIDs;
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
 * Calculates the value of a subject symptom
 * 
 */
int SolvingMachine::calculateValue(const Symptom& symptom, const ProblemMap& subjectProblems)
{
    /** \todo Lubo: this should also take in account negative symptoms and problems!!! */
    /** \todo Lubo: implement symptom calculate value!! */
    return 0;
}

/**
 * Calculates the value of a subject problem by the probability it has to be positive
 */
int SolvingMachine::calculateValue(const Problem& problem, const SymptomsWithSameProblem& connectedSymptoms, const SymptomMap& positiveSymptoms)
{
    /** \todo Lubo: this should also take in account negative symptoms and problems!!! */
    int valueOfProblem = 0;
    
    boost::unordered_set<int> missingSymptoms;
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
    
    valueOfProblem = maxHint*totalValue;
    
    BOOST_FOREACH(int missingSymptomID, missingSymptoms)
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
