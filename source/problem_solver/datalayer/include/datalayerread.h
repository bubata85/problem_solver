/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "category.h"
#include "problem.h"
#include "symptom.h"
#include "solution.h"

#include "investigation.h"

#include "baseexception.h"

#include <vector>
#include <boost/unordered_map.hpp>

namespace ProblemSolver
{

// Object ID to Object maps
typedef boost::unordered_map<Identifier, Category> CategoryMap;
typedef boost::unordered_map<Identifier, Problem> ProblemMap;
typedef boost::unordered_map<Identifier, Symptom> SymptomMap;
typedef boost::unordered_map<Identifier, Solution> SolutionMap;

typedef boost::unordered_map<Identifier, ExtendedProblem> ExtendedProblemMap;
typedef boost::unordered_map<Identifier, ExtendedSymptom> ExtendedSymptomMap;
typedef boost::unordered_map<Identifier, ExtendedSolution> ExtendedSolutionMap;

typedef boost::unordered_map<Identifier, SymptomLink> SymptomLinkMap;
typedef boost::unordered_map<Identifier, SolutionLink> SolutionLinkMap;

typedef boost::unordered_map<Identifier, Investigation> InvestigationMap;

// all links connected with a certain problem, organized by symptom ID
typedef boost::unordered_map<Identifier, SymptomLink> SymptomsWithSameProblem;

// all links connected with a certain symptom, organized by problem ID
typedef boost::unordered_map<Identifier, SymptomLink> ProblemsWithSameSymptom;

// all links connected with a certain problem, organized by solution ID
typedef boost::unordered_map<Identifier, SolutionLink> SolutionsWithSameProblem;

// all links connected with a certain solution, organized by problem ID
typedef boost::unordered_map<Identifier, SolutionLink> ProblemsWithSameSolution;

/**
 * Exception thrown from all DataLayer operations
 */
class DataLayerException: public BaseException
{
public:
    explicit DataLayerException(const std::string& errorMessage):
        BaseException(errorMessage){}
        
    virtual ExceptionCode getCode() const { return exceptionCodeDataLayer; }
};

/**
 * Interface for accessing the DataLayer for read-only operations
 */
class IDataLayerRead
{
public:
    
    virtual ~IDataLayerRead(){}
    
public:
    
    /**
     * If no IDs are specified the functions will return ALL objects
     * The notFound vector will contains the ID's of objects that were not found.
     * If notFound is NULL, the function will throw if something is not found.
     */
    virtual void get(const std::vector<Identifier>& categoryIDs, CategoryMap& result, std::vector<Identifier>* notFound = NULL) = 0;
    virtual void get(const std::vector<Identifier>& problemIDs, ProblemMap& result, std::vector<Identifier>* notFound = NULL) = 0;
    virtual void get(const std::vector<Identifier>& symptomIDs, SymptomMap& result, std::vector<Identifier>* notFound = NULL) = 0;
    virtual void get(const std::vector<Identifier>& solutionIDs, SolutionMap& result, std::vector<Identifier>* notFound = NULL) = 0;
    virtual void get(const std::vector<Identifier>& symptomLinkIDs, SymptomLinkMap& result, std::vector<Identifier>* notFound = NULL) = 0;
    virtual void get(const std::vector<Identifier>& solutionLinkIDs, SolutionLinkMap& result, std::vector<Identifier>* notFound = NULL) = 0;
    virtual void get(const std::vector<Identifier>& investigationIDs, InvestigationMap& result, std::vector<Identifier>* notFound = NULL) = 0;
    
    /**
     * Get Extended version of objects
     */
    virtual void get(const std::vector<Identifier>& problemIDs, ExtendedProblemMap& result, std::vector<Identifier>* notFound = NULL) = 0;
    virtual void get(const std::vector<Identifier>& symptomIDs, ExtendedSymptomMap& result, std::vector<Identifier>* notFound = NULL) = 0;
    virtual void get(const std::vector<Identifier>& solutionIDs, ExtendedSolutionMap& result, std::vector<Identifier>* notFound = NULL) = 0;
    
    /**
     * The found output value will contain true false depending if the object was found
     * If found is NULL, the function will throw if something is not found
     */
    virtual void getLinksByProblem(Identifier problemID, SymptomsWithSameProblem& result, bool* found = NULL) = 0;
    virtual void getLinksBySymptom(Identifier symptomID, ProblemsWithSameSymptom& result, bool* found = NULL) = 0;
    
    virtual void getLinksByProblem(Identifier problemID, SolutionsWithSameProblem& result, bool* found = NULL) = 0;
    virtual void getLinksBySolution(Identifier solutionID, ProblemsWithSameSolution& result, bool* found = NULL) = 0;
    
};

} // namespace ProblemSolver
