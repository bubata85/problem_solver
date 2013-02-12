/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "datalayerread.h"

#include <vector>
#include <boost/unordered_set.hpp>

namespace ProblemSolver
{

/**
 * This class groups all functionality connected with making suggestions about how to identify unknown problems
 */
class SolvingMachine
{
public:
    
    /**
     * If the "highest" reference inside a link is above this number it will be treated as this much,
     * and the rest of the references will be diminished by a coefficient equal to that needed
     * to reduce the "highest" reference to this number.
     * E.g: if MAX_REFERENCES is 100
     * if we have a link: positive 400, false-positive 200, negative 40
     * then they will be transformed to: positive 100, false-positive 50, negative 10
     */
    static const double MAX_REFERENCES = 100;
    
public:
    
    SolvingMachine(IDataLayerRead& dataLayer);
    ~SolvingMachine(){}

public:
    
    /**
     * Exception thrown from SolvingMachine operations
     */
    class Exception: public BaseException
    {
    public:
        explicit Exception(const std::string& errorMessage):
            BaseException(errorMessage){}
            
        virtual ExceptionCode getCode() const { return exceptionCodeSolvingMachine; }
    };
    
public:
    
    /**
     * This is the result of a makeSuggestion call.
     * It provides the system's opinion about what should be made next.
     */
    struct Suggestion
    {
        std::vector<int> symptoms; // list of symptoms that can be checked
        std::vector<int> symptomValues; // numeric representation of how worth it is to check a symptom
        
        std::vector<int> problems; // list of problems that can be checked
        std::vector<int> problemValues; // numeric representation of how worth it is to check a problem
        
        std::vector<int> solutions; // list of solutions that can be applied
        std::vector<int> solutionValues; // numeric representation of how worth it is to apply a solution
    };
    
public:
    
    Suggestion makeSuggestion(const Investigation& Investigation);

private:
    
    typedef boost::unordered_set<int> CategoryBranch;
    CategoryBranch buildCategoryBranch(CategoryBranch partialBranch);
    void addChilds(int categoryID, CategoryBranch& result, const CategoryMap& allCategories);
    
    template< class InputLinks, class OutputObjects>
    void filterByBranch(const InputLinks& relatedLinks, const CategoryBranch& categoryBranch, OutputObjects& result);
    
private:
    
    int calculateValue(const Solution& solution, const SolutionLink& link);
    
private:
    
    IDataLayerRead& _dataLayer;
    
};

} // namespace ProblemSolver
