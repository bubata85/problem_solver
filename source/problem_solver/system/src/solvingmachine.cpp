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
            if(utils::contains(investigation.positiveSymptoms, symptom.id) ||
               utils::contains(investigation.negativeSymptoms, symptom.id) ||
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
                if(utils::contains(investigation.negativeSolutions, solution.id) ||
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
            if(utils::contains(investigation.negativeProblems, problem.id) ||
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
        
        // problems to its positive symptoms
        boost::unordered_map<int, ProblemValue> problemToValue;

        // positive symptoms to their links
        boost::unordered_map<int, ProblemsWithSameSymptom> symptomsPositiveToLinks;
        
        // negative symptoms to their links
        boost::unordered_map<int, ProblemsWithSameSymptom> symptomsNegativeToLinks;
        
        // retrieve positive links
        BOOST_FOREACH(const SymptomMap::value_type& pair, positiveSymptoms)
        {
            ProblemsWithSameSymptom& relatedProblems = symptomsPositiveToLinks[pair.second.id];
            _dataLayer.getLinksBySymptom(pair.second.id, relatedProblems);
            
            //BOOST_FOREACH(const ProblemsWithSameSymptom::value_type& pair, relatedProblems)
            {
                //const SymptomLink& link = pair.second;
                //ProblemValue& value = problemToValue[link.problemID];
                
                
            }
        }
        
        // retrieve negative links
        BOOST_FOREACH(const SymptomMap::value_type& pair, negativeSymptoms)
        {
            ProblemsWithSameSymptom& relatedProblems = symptomsNegativeToLinks[pair.second.id];
            _dataLayer.getLinksBySymptom(pair.second.id, relatedProblems);
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
template< class InputLinks, class OutputObjects>
void SolvingMachine::filterByBranch(const InputLinks& relatedLinks, const CategoryBranch& categoryBranch, OutputObjects& result)
{
    std::vector<int> objectIDs;
    BOOST_FOREACH(const typename InputLinks::value_type& pair, relatedLinks)
    {
        objectIDs.push_back(pair.first);
    }
    
    OutputObjects allObjects;
    _dataLayer.get(objectIDs, allObjects);
    
    BOOST_FOREACH(const typename OutputObjects::value_type& pair, allObjects)
    {
        // add only relevant symptoms
        if(categoryBranch.find(pair.second.categoryID) != categoryBranch.end())
            result.insert(pair);
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
 * Calculates the value of a solution-problem link
 */
int SolvingMachine::calculateValue(const GenericInfo& object, const SolutionLink& link)
{
    double greatest = std::max(link.positive, link.negative);
    double reduction = 1;
    if(greatest > MAX_REFERENCES)
        reduction = MAX_REFERENCES/greatest;
    
    double positive = link.positive*reduction;
    double negative = link.negative*reduction;
    double value = (positive * positive) / (positive + negative);
    
    if(!link.confirmed)
        value /= 2; // apply penalty for non-confirmed links
    
    if(!object.confirmed)
        value /= 2; // apply penalty for non-confirmed solution
        
    return value;
}

} // namespace ProblemSolver
