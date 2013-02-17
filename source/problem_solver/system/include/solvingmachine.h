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
    static const double UNCONFIRMED_PENALTY = 0.5; // penalty applied to links and objects that are not yet confirmed
    static const double MISSING_SYMPTOM_PENALTY = 0.5; // penalty applied to problems for each missing confirmed symptom
    static const double MISSING_UNCONFIRMED_SYMPTOM_PENALTY = 0.75; // less restrictive
    /**
     * A problem is considered part of the upper bound when the difference in value between it and
     * the problem with highest value is less than this constant:
     * E.g. if we have 4 problems with values 75, 50, 44, 30 then the upper bound problems are those with values 75 and 50.
     */
    static const double UPPER_BOUND_PROBLEM_RANGE = 20; 
    
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
        std::vector<Identifier> symptoms; // list of symptoms that can be checked
        std::vector<int> symptomValues; // numeric representation of how worth it is to check a symptom
        
        std::vector<Identifier> problems; // list of problems that can be checked
        std::vector<int> problemValues; // numeric representation of how worth it is to check a problem
        
        std::vector<Identifier> solutions; // list of solutions that can be applied
        std::vector<int> solutionValues; // numeric representation of how worth it is to apply a solution
    };
    
public:
    
    Suggestion makeSuggestion(const Investigation& Investigation);
    
private:
    
    // set of all the category IDs that define a working branch
    typedef boost::unordered_set<Identifier> CategoryBranch;
    
    // key is symptom ID, value is all the problems linked to the symptom
    typedef boost::unordered_map<Identifier, ProblemsWithSameSymptom> SymptomToLinks;
    
    // key is problem ID, value is all the symptoms linked to the problem
    typedef boost::unordered_map<Identifier, SymptomsWithSameProblem> ProblemToLinks;
    
private:
    
    CategoryBranch buildCategoryBranch(CategoryBranch partialBranch);
    void addChilds(CIdentifier categoryID, CategoryBranch& result, const CategoryMap& allCategories);
    
    template< class Links, class Objects>
    void filterByBranch(const Links& relatedLinks, const CategoryBranch& categoryBranch, Objects& result);
    template<class Objects>
    void filterByBranch(const CategoryBranch& categoryBranch, Objects& result);
    
private:
    
    double getDifficultyPenalty(DifficultyLevel level);
    double calculateAccuracity(int firstReferences, int secondReferences);
    double calculateReduction(int firstReferences, int secondReferences);
    double calculateValue(int positiveReferences, int negativeReferences);
    
    int calculateValue(const GenericInfo& object, const SolutionLink& link);
    int calculateValue(const Symptom& symptom, const ProblemMap& subjectProblems, const ProblemToLinks& allProblemLinks,
                       const SymptomMap& positiveSymptoms, const Suggestion& suggestion);
    int calculateValue(const Problem& problem, const SymptomsWithSameProblem& connectedSymptoms, const SymptomMap& positiveSymptoms);
    
private:
    
    IDataLayerRead& _dataLayer;
    
};

} // namespace ProblemSolver
