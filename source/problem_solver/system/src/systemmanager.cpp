/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "systemmanager.h"

namespace ProblemSolver
{

/**
 * Returns a suggested continue path for identifying the input unknown problem.
 */
SystemManager::SystemManager(IDataLayer* dataLayer):
    _dataLayer(dataLayer)
{
}

/**
 * Searches for all problems symptoms and solutions that match the search phrase and their
 */
SystemManager::SearchResult SystemManager::performSearch(const std::string& searchPhrase, int chosenCategory)
{
    SearchResult searchResult;
    
    assert(false && "NYI");
    
    return searchResult;
}

/**
 * This function should be called every time a new problem has been checked in an investigation.
 * It will update the investigation and save any related information.
 */
void SystemManager::onProblemChecked(int problemID, bool checkResult, int investigationID)
{
    Investigation investigation = getInvestigation(investigationID);
    
    // check if the investigation has positive problem
    if(checkResult == true && investigation.positiveProblem != -1)
        throw Exception("Investigation already has a positive problem!");
    
    // check if the investigation has this negative problem
    for(uint i = 0; i < investigation.negativeProblems.size(); ++i)
    {
        if(problemID == investigation.negativeProblems[i])
            throw Exception("Investigation has already checked this problem!");
    }
    
    if(problemID == investigation.positiveProblem)
        throw Exception("Investigation has already checked this problem!");
    
    // always update positive symptoms, update negative symptoms only if the problem was positive
    if(!investigation.positiveSymptoms.empty() || (checkResult == true && !investigation.negativeSymptoms.empty()))
    {
        SymptomsWithSameProblem relatedSymptoms;
        _dataLayer->getLinksByProblem(problemID, relatedSymptoms);
        
        if(checkResult == false)
        {
            // increment the false-positive value of links to positive symptoms
            updateLinks(true, problemID, investigation.positiveSymptoms, relatedSymptoms, symptomLinkAddFalsePositive);
        }
        else
        {
            // increment the positive value of links to positive symptoms
            updateLinks(true, problemID, investigation.positiveSymptoms, relatedSymptoms, symptomLinkAddPositive);
            
            // increment the negative value of links to negative symptoms
            updateLinks(true, problemID, investigation.negativeSymptoms, relatedSymptoms, symptomLinkAddNegative);
        }
    }
    
    // solutions links are only updated when we have a problem
    if(checkResult == true && (investigation.positiveSolution != -1 || !investigation.negativeSolutions.empty()))
    {
        SolutionsWithSameProblem relatedSolutions;
        _dataLayer->getLinksByProblem(problemID, relatedSolutions);
        
        // increment the positive solution
        if(investigation.positiveSolution != -1)
        {
            std::vector<int> positiveSolution;
            positiveSolution.push_back(investigation.positiveSolution);
            updateLinks(true, problemID, positiveSolution, relatedSolutions, solutionLinkAddPositive);
        }
        
        updateLinks(true, problemID, investigation.negativeSolutions, relatedSolutions, solutionLinkAddNegative);
    }
    
    // update the investigation itself
    if(checkResult == true)
        investigation.positiveProblem = problemID;
    else
        investigation.negativeProblems.push_back(problemID);
    
    // clear the problem from the "banned" symptoms
    for(uint i = 0; i < investigation.bannedProblems.size(); ++i)
    {
        if(problemID == investigation.bannedProblems[i])
        {
            investigation.bannedProblems.erase(investigation.bannedProblems.begin() + i);
            break;
        }
    }
    
    _dataLayer->modify(investigation);
}

/**
 * This function should be called every time a new symptom has been checked in an investigation.
 * It will update the investigation and save any related information.
 */
void SystemManager::onSymptomChecked(int symptomID, bool checkResult, int investigationID)
{
    Investigation investigation = getInvestigation(investigationID);
    
    // check if the investigation has this positive symptom
    for(uint i = 0; i < investigation.positiveSymptoms.size(); ++i)
    {
        if(symptomID == investigation.positiveSymptoms[i])
            throw Exception("Investigation has already checked this symptom!");
    }
    
    // check if the investigation has this negative symptom
    for(uint i = 0; i < investigation.negativeSymptoms.size(); ++i)
    {
        if(symptomID == investigation.negativeSymptoms[i])
            throw Exception("Investigation has already checked this symptom!");
    }
    
    // always update positive problem, update negative problems only if the symptom was positive
    if(investigation.positiveProblem != -1 || (checkResult == true && !investigation.negativeProblems.empty()))
    {
        ProblemsWithSameSymptom relatedProblems;
        _dataLayer->getLinksBySymptom(symptomID, relatedProblems);
        
        if(investigation.positiveProblem != -1)
        {
            std::vector<int> positiveProblem;
            positiveProblem.push_back(investigation.positiveProblem);

            // increment the positive/negative value of the link to the positive problem
            updateLinks(false, symptomID, positiveProblem, relatedProblems, checkResult?symptomLinkAddPositive:symptomLinkAddNegative);
        }
        
        if(checkResult == true)
        {
            // increment the false-positive value of links to negative problems
            updateLinks(false, symptomID, investigation.negativeProblems, relatedProblems, symptomLinkAddFalsePositive);
        }
    }
    
    // update the investigation itself
    if(checkResult == true)
        investigation.positiveSymptoms.push_back(symptomID);
    else
        investigation.negativeSymptoms.push_back(symptomID);
    
    // clear the symptom from the "banned" symptoms
    for(uint i = 0; i < investigation.bannedSymptoms.size(); ++i)
    {
        if(symptomID == investigation.bannedSymptoms[i])
        {
            investigation.bannedSymptoms.erase(investigation.bannedSymptoms.begin() + i);
            break;
        }
    }
    
    _dataLayer->modify(investigation);
}

/**
 * This function should be called every time a new solution has been checked in an investigation.
 * It will update the investigation and save any related information.
 */
void SystemManager::onSolutionChecked(int solutionID, bool checkResult, int investigationID)
{
    Investigation investigation = getInvestigation(investigationID);
    
    // check if the investigation has positive solution
    if(checkResult == true && investigation.positiveSolution != -1)
        throw Exception("Investigation already has a positive solution!");
    
    // check if the investigation has this as positive solution
    if(solutionID == investigation.positiveSolution)
        throw Exception("Investigation has already checked this solution!");
    
    // check if the investigation has this negative solution
    for(uint i = 0; i < investigation.negativeSolutions.size(); ++i)
    {
        if(solutionID == investigation.negativeSolutions[i])
            throw Exception("Investigation has already checked this solution!");
    }
    
    // update only when we have a positive problem
    if(investigation.positiveProblem != -1)
    {
        ProblemsWithSameSolution relatedProblems;
        _dataLayer->getLinksBySolution(solutionID, relatedProblems);
        
        std::vector<int> positiveProblem;
        positiveProblem.push_back(investigation.positiveProblem);

        // increment the positive/negative value of the link to the positive problem
        updateLinks(false, solutionID, positiveProblem, relatedProblems, checkResult?solutionLinkAddPositive:solutionLinkAddNegative);
    }
    
    // update the investigation itself
    if(checkResult == true)
        investigation.positiveSolution = solutionID;
    else
        investigation.negativeSolutions.push_back(solutionID);
    
    // clear the solution from the "banned" solutions
    for(uint i = 0; i < investigation.bannedSolutions.size(); ++i)
    {
        if(solutionID == investigation.bannedSolutions[i])
        {
            investigation.bannedSolutions.erase(investigation.bannedSolutions.begin() + i);
            break;
        }
    }
    
    _dataLayer->modify(investigation);
}

/**
 * Returns a suggested course of action based on the current state of an unknown problem
 */
SolvingMachine::Suggestion SystemManager::makeSuggestion(const Investigation& investigation)
{
    return SolvingMachine::makeSuggestion(investigation, *_dataLayer);
}

/**
 * Gain access to the data layer being used to meddle with it as you like
 */
IDataLayer& SystemManager::getDataLayer()
{
    return *_dataLayer;
}

/**
 * Updates the correct attribute of a link based on the action enum
 */
void SystemManager::updateLinkByAction(SymptomLink& link, SymptomLinkAction action)
{
    switch(action)
    {
    case symptomLinkAddFalsePositive:
        link.falsePositiveChecks += 1;
        break;
    case symptomLinkAddPositive:
        link.positiveChecks += 1;
        break;
    case symptomLinkAddNegative:
        link.negativeChecks += 1;
        break;
    default:
        assert(false && "unknown update symptom link action");
    }
}

/**
 * Updates the correct attribute of a link based on the action enum
 */
void SystemManager::updateLinkByAction(SolutionLink& link, SolutionLinkAction action)
{
    switch(action)
    {
    case solutionLinkAddPositive:
        link.positive += 1;
        break;
    case solutionLinkAddNegative:
        link.negative += 1;
        break;
    default:
        assert(false && "unknown update solution link action");
    }
}

/**
 * Performs the supplied action on all inputLinks inside allLinks and the database.
 * If a link is missing it will be automatically added
 * If byProblem is true then relatedID will be a problemID and the map will contain symptomIDs.
 * If it is false then it will be vice-versa.
 */
void SystemManager::updateLinks(bool byProblem, int relatedID, const std::vector<int>& inputLinks,
                                boost::unordered_map<int, SymptomLink>& allLinks, SymptomLinkAction action)
{
    // iterate the inputLink
    for(uint i = 0; i < inputLinks.size(); ++i)
    {
        boost::unordered_map<int, SymptomLink>::iterator link = allLinks.find(inputLinks[i]);
        if(link == allLinks.end())
        {
            // this is a new link
            SymptomLink newLink;
            
            if(byProblem)
            {
                newLink.problemID = relatedID;
                newLink.symptomID = inputLinks[i];
            }
            else
            {
                newLink.problemID = inputLinks[i];
                newLink.symptomID = relatedID;
            }
            
            updateLinkByAction(newLink, action);
            
            // save in data layer
            newLink.id = _dataLayer->add(newLink);
            
            // save in our copy
            allLinks[inputLinks[i]] = newLink;
        }
        else
        {
            // this link already exists
            updateLinkByAction(link->second, action);
            _dataLayer->modify(link->second);
        }
    }
}

/**
 * Performs the supplied action on all inputLinks inside allLinks and the database.
 * If a link is missing it will be automatically added
 * If byProblem is true then relatedID will be a problemID and the map will contain solutionIDs.
 * If it is false then it will be vice-versa.
 */
void SystemManager::updateLinks(bool byProblem, int relatedID, const std::vector<int>& inputLinks,
                                boost::unordered_map<int, SolutionLink>& allLinks, SolutionLinkAction action)
{
    // iterate the inputLink
    for(uint i = 0; i < inputLinks.size(); ++i)
    {
        boost::unordered_map<int, SolutionLink>::iterator link = allLinks.find(inputLinks[i]);
        if(link == allLinks.end())
        {
            // this is a new link
            SolutionLink newLink;
            
            if(byProblem)
            {
                newLink.problemID = relatedID;
                newLink.solutionID = inputLinks[i];
            }
            else
            {
                newLink.problemID = inputLinks[i];
                newLink.solutionID = relatedID;
            }
            
            updateLinkByAction(newLink, action);
            
            // save in data layer
            newLink.id = _dataLayer->add(newLink);
            
            // save in our copy
            allLinks[inputLinks[i]] = newLink;
        }
        else
        {
            // this link already exists
            updateLinkByAction(link->second, action);
            _dataLayer->modify(link->second);
        }
    }
}

/**
 * Returns the investigation that corresponds to the supplied ID
 */
Investigation SystemManager::getInvestigation(int investigationID)
{
    std::vector<int> ids;
    ids.push_back(investigationID);
    
    InvestigationMap investigationMap;
    _dataLayer->get(ids, investigationMap);
    
    return investigationMap.begin()->second;
}
    
} // namespace ProblemSolver
