/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "solvingmachine.h"

#include <auto_ptr.h>
#include <vector>
#include <string>

namespace ProblemSolver
{

/**
 * This is a remote interface to a system manager that communicates using JSON.
 */
class RemoteJsonManager
{
public:
    
    RemoteJsonManager(IDataLayer* dataLayer);
    ~RemoteJsonManager(){};
    
public:
    
    /**
     * This is the result of a search
     */
    struct SearchResult
    {
        std::vector<int> problems;
        std::vector<int> symptoms;
        std::vector<int> solutions;
    };
    
public:
    
    SearchResult performSearch(const std::string& searchPhrase, int chosenCategory);
    
    /**
     * For all of these it does not matter if the check result has been updated inside the Investigation or not,
     * in all cases it will be treated as if it was.
     */
    void onProblemChecked(int problemID, bool checkResult, const Investigation& investigation);
    void onSymptomChecked(int symptomID, bool checkResult, const Investigation& investigation);
    void onSolutionChecked(int solutionID, bool checkResult, const Investigation& investigation);
    
    SolvingMachine::Suggestion makeSuggestion(const Investigation& investigation);
    IDataLayer& getDataLayer();
    
private:
    
    std::auto_ptr<IDataLayer> _dataLayer;

};

} // namespace ProblemSolver
