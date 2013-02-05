/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "solvingmachine.h"
#include "baseexception.h"

#include <auto_ptr.h>
#include <vector>
#include <string>

namespace ProblemSolver
{

class IDataLayer;
class Investigation;
    
/**
 * This is the main class used to perform tasks in the system.
 * It takes ownership on the supplied data layer and works with it.
 */
class SystemManager
{
public:
    
    SystemManager(IDataLayer* dataLayer);
    ~SystemManager(){};
    
public:
    
    /**
     * Exception thrown from SystemManager operations
     */
    class Exception: public BaseException
    {
    public:
        explicit Exception(const std::string& errorMessage):
            BaseException(errorMessage){}
            
        virtual ExceptionCode getCode() const { return exceptionCodeSystemManager; }
    };
    
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
