/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "systemmanager.h"

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

namespace ProblemSolver
{

/**
 * Returns a suggested continue path for identifying the input unknown problem.
 */
SystemManager::SystemManager(IDataLayer* dataLayer):
    _dataLayer(dataLayer)
{
    if(dataLayer == NULL)
        throw Exception("SystemManager: Cannot use NULL dataLayer");
}

/**
 * Searches for all problems symptoms and solutions that match the search phrase and their
 */
SystemManager::SearchResult SystemManager::performSearch(const std::string& searchPhrase)
{
    /** \todo Lubo: include a category branch in the search! */
    
    SearchResult searchResult;
    
    std::vector<std::string> inputWords;
    boost::split(inputWords, searchPhrase, boost::is_any_of(" "));
    
    std::vector<std::string> searchWords;
    BOOST_FOREACH(std::string& word, inputWords)
    {
        if(word.size() > 2)
        {
            /** \todo Lubo: this needs to be re-enabled when DB starts saving lowercase */
            //boost::algorithm::to_lower(word);
            searchWords.push_back(word);
        }
    }
    
    // search through all symptoms
    populateSearchResult<ExtendedSymptomMap>(searchWords, searchResult.symptoms, searchResult.symptomRelevance);
    
    // search through all problems
    populateSearchResult<ExtendedProblemMap>(searchWords, searchResult.problems, searchResult.problemRelevance);
    
    // search through all solutions
    populateSearchResult<ExtendedSolutionMap>(searchWords, searchResult.solutions, searchResult.solutionRelevance);
    
    return searchResult;
}

/**
 * This function should be called every time a new problem has been checked in an investigation.
 * It will update the investigation and save any related information.
 */
void SystemManager::onProblemChecked(CIdentifier problemID, bool checkResult, CIdentifier investigationID)
{
    Investigation investigation = getInvestigation(investigationID);
    
    // check if the investigation has positive problem
    if(checkResult == true && !investigation.positiveProblem.empty())
        throw Exception("SystemManager: Investigation already has a positive problem!");
    
    // check if the investigation has this negative problem
    for(uint i = 0; i < investigation.negativeProblems.size(); ++i)
    {
        if(problemID == investigation.negativeProblems[i])
            throw Exception("SystemManager: Investigation has already checked this problem!");
    }
    
    if(problemID == investigation.positiveProblem)
        throw Exception("SystemManager: Investigation has already checked this problem!");
    
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
    if(checkResult == true && (!investigation.positiveSolution.empty() || !investigation.negativeSolutions.empty()))
    {
        SolutionsWithSameProblem relatedSolutions;
        _dataLayer->getLinksByProblem(problemID, relatedSolutions);
        
        // increment the positive solution
        if(!investigation.positiveSolution.empty())
        {
            std::vector<Identifier> positiveSolution;
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
void SystemManager::onSymptomChecked(CIdentifier symptomID, bool checkResult, CIdentifier investigationID)
{
    Investigation investigation = getInvestigation(investigationID);
    
    // check if the investigation has this positive symptom
    for(uint i = 0; i < investigation.positiveSymptoms.size(); ++i)
    {
        if(symptomID == investigation.positiveSymptoms[i])
            throw Exception("SystemManager: Investigation has already checked this symptom!");
    }
    
    // check if the investigation has this negative symptom
    for(uint i = 0; i < investigation.negativeSymptoms.size(); ++i)
    {
        if(symptomID == investigation.negativeSymptoms[i])
            throw Exception("SystemManager: Investigation has already checked this symptom!");
    }
    
    // always update positive problem, update negative problems only if the symptom was positive
    if(!investigation.positiveProblem.empty() || (checkResult == true && !investigation.negativeProblems.empty()))
    {
        ProblemsWithSameSymptom relatedProblems;
        _dataLayer->getLinksBySymptom(symptomID, relatedProblems);
        
        if(!investigation.positiveProblem.empty())
        {
            std::vector<Identifier> positiveProblem;
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
void SystemManager::onSolutionChecked(CIdentifier solutionID, bool checkResult, CIdentifier investigationID)
{
    Investigation investigation = getInvestigation(investigationID);
    
    // check if the investigation has positive solution
    if(checkResult == true && !investigation.positiveSolution.empty())
        throw Exception("SystemManager: Investigation already has a positive solution!");
    
    // check if the investigation has this as positive solution
    if(solutionID == investigation.positiveSolution)
        throw Exception("SystemManager: Investigation has already checked this solution!");
    
    // check if the investigation has this negative solution
    for(uint i = 0; i < investigation.negativeSolutions.size(); ++i)
    {
        if(solutionID == investigation.negativeSolutions[i])
            throw Exception("SystemManager: Investigation has already checked this solution!");
    }
    
    // update only when we have a positive problem
    if(!investigation.positiveProblem.empty())
    {
        ProblemsWithSameSolution relatedProblems;
        _dataLayer->getLinksBySolution(solutionID, relatedProblems);
        
        std::vector<Identifier> positiveProblem;
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
SolvingMachine::Suggestion SystemManager::makeSuggestion(CIdentifier investigationID)
{
    Investigation investigation = getInvestigation(investigationID);
    
    SolvingMachine machine(*_dataLayer.get());
    return machine.makeSuggestion(investigation);
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
        throw Exception("SystemManager: Unknown update symptom link action!");
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
        throw Exception("SystemManager: Unknown update solution link action!");
    }
}

/**
 * Performs the supplied action on all inputLinks inside allLinks and the database.
 * If a link is missing it will be automatically added
 * If byProblem is true then relatedID will be a problemID and the map will contain symptomIDs.
 * If it is false then it will be vice-versa.
 */
void SystemManager::updateLinks(bool byProblem, CIdentifier relatedID, const std::vector<Identifier>& inputLinks,
                                boost::unordered_map<Identifier, SymptomLink>& allLinks, SymptomLinkAction action)
{
    // iterate the inputLink
    for(uint i = 0; i < inputLinks.size(); ++i)
    {
        boost::unordered_map<Identifier, SymptomLink>::iterator link = allLinks.find(inputLinks[i]);
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
void SystemManager::updateLinks(bool byProblem, CIdentifier relatedID, const std::vector<Identifier>& inputLinks,
                                boost::unordered_map<Identifier, SolutionLink>& allLinks, SolutionLinkAction action)
{
    // iterate the inputLink
    for(uint i = 0; i < inputLinks.size(); ++i)
    {
        boost::unordered_map<Identifier, SolutionLink>::iterator link = allLinks.find(inputLinks[i]);
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
Investigation SystemManager::getInvestigation(CIdentifier investigationID)
{
    std::vector<Identifier> ids;
    ids.push_back(investigationID);
    
    InvestigationMap investigationMap;
    _dataLayer->get(ids, investigationMap);
    
    return investigationMap.begin()->second;
}

/**
 * Populates the provided vectors with the IDs and relevance of all objects of type T that have at least
 * one tag matching the input search words.
 */
template<class T>
void SystemManager::populateSearchResult(const std::vector<std::string> searchWords, std::vector<Identifier>& objectIDs, std::vector<int>& objectRelevance)
{
    // search through all objects
    T allObjects;
    _dataLayer->get(std::vector<Identifier>(), allObjects);
    
    BOOST_FOREACH(typename T::value_type& pair, allObjects)
    {
        int matchingWords = 0;
        BOOST_FOREACH(const std::string& searchWord, searchWords)
        {
            if(pair.second.tags.find(searchWord) != pair.second.tags.end())
                ++matchingWords;
        }
        
        if(matchingWords > 0)
        {
            objectIDs.push_back(pair.second.id);
            objectRelevance.push_back(100*((double)matchingWords / (double)searchWords.size()));
        }
    }
}
    
} // namespace ProblemSolver
