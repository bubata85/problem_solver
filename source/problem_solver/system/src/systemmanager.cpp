/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "systemmanager.h"
#include "datalayer.h"

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

void SystemManager::onProblemChecked(int problemID, bool checkResult, const Investigation& investigation)
{
    if(checkResult == true && investigation.positiveProblem != -1 && problemID != investigation.positiveProblem)
        throw Exception("Investigation already has a positive problem!");
    
    // positive symptoms always update links, while negative symptoms update links only if the problem was positive
    if(!investigation.positiveSymptoms.empty() || (checkResult == true && !investigation.negativeSymptoms.empty()))
    {
        SymptomsByProblemMap symptomLinks;
        _dataLayer->getLinksByProblem(problemID, symptomLinks);
    }
    
    
}

void SystemManager::onSymptomChecked(int symptomID, bool checkResult, const Investigation& investigation)
{
    
}

void SystemManager::onSolutionChecked(int solutionID, bool checkResult, const Investigation& investigation)
{
    
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

    
} // namespace ProblemSolver
