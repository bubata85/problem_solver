/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "solvingmachine.h"
#include "datalayer.h"

#include <auto_ptr.h>
#include <vector>
#include <string>
#include <boost/unordered_map.hpp>

namespace ProblemSolver
{

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
    void onProblemChecked(int problemID, bool checkResult, int investigationID);
    void onSymptomChecked(int symptomID, bool checkResult, int investigationID);
    void onSolutionChecked(int solutionID, bool checkResult, int investigationID);
    
    SolvingMachine::Suggestion makeSuggestion(const Investigation& investigation);
    IDataLayer& getDataLayer();
    
private:
    
    /**
     * Update Actions that can be performed on a symptomLink
     */
    enum SymptomLinkAction
    {
        symptomLinkAddFalsePositive,
        symptomLinkAddPositive,
        symptomLinkAddNegative
    };
    
    /**
     * Update Actions that can be performed on a solutionLink
     */
    enum SolutionLinkAction
    {
        solutionLinkAddPositive,
        solutionLinkAddNegative
    };
    
    void updateLinkByAction(SymptomLink& link, SymptomLinkAction action);
    void updateLinkByAction(SolutionLink& link, SolutionLinkAction action);
    
    void updateLinks(bool byProblem, int relatedID, const std::vector<int>& inputLinks,
                     boost::unordered_map<int, SymptomLink>& allLinks, SymptomLinkAction action);
    
    void updateLinks(bool byProblem, int relatedID, const std::vector<int>& inputLinks,
                     boost::unordered_map<int, SolutionLink>& allLinks, SolutionLinkAction action);
    
    Investigation getInvestigation(int investigationID);
    
private:
    
    std::auto_ptr<IDataLayer> _dataLayer;

};

} // namespace ProblemSolver
