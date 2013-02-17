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
        std::vector<Identifier> symptoms;
        std::vector<int> symptomRelevance;
        std::vector<Identifier> problems;
        std::vector<int> problemRelevance;
        std::vector<Identifier> solutions;
        std::vector<int> solutionRelevance;
    };

public:
    
    SearchResult performSearch(const std::string& searchPhrase);
    
    /**
     * For all of these it does not matter if the check result has been updated inside the Investigation or not,
     * in all cases it will be treated as if it was.
     */
    /** \todo Lubo: links updated by events must adapt values so that they always have a chance to turn upside down given time */
    /** \todo Lubo: automatically return suggestion? */
    void onProblemChecked(CIdentifier problemID, bool checkResult, CIdentifier investigationID);
    void onSymptomChecked(CIdentifier symptomID, bool checkResult, CIdentifier investigationID);
    void onSolutionChecked(CIdentifier solutionID, bool checkResult, CIdentifier investigationID);
    
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
    
    void updateLinks(bool byProblem, CIdentifier relatedID, const std::vector<Identifier>& inputLinks,
                     boost::unordered_map<Identifier, SymptomLink>& allLinks, SymptomLinkAction action);
    
    void updateLinks(bool byProblem, CIdentifier relatedID, const std::vector<Identifier>& inputLinks,
                     boost::unordered_map<Identifier, SolutionLink>& allLinks, SolutionLinkAction action);
    
    Investigation getInvestigation(CIdentifier investigationID);
    
    template<class T>
    void populateSearchResult(const std::vector<std::string> searchWords, std::vector<Identifier>& objectIDs, std::vector<int>& objectRelevance);
    
private:
    
    std::auto_ptr<IDataLayer> _dataLayer;

};

} // namespace ProblemSolver
