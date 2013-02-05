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
typedef boost::unordered_map<int, Category> CategoryMap;
typedef boost::unordered_map<int, Problem> ProblemMap;
typedef boost::unordered_map<int, Symptom> SymptomMap;
typedef boost::unordered_map<int, Solution> SolutionMap;

typedef boost::unordered_map<int, ExtendedProblem> ExtendedProblemMap;
typedef boost::unordered_map<int, ExtendedSymptom> ExtendedSymptomMap;
typedef boost::unordered_map<int, ExtendedSolution> ExtendedSolutionMap;

typedef boost::unordered_map<int, Investigation> InvestigationMap;

// all links connected with a certain problem, organized by symptom ID
typedef boost::unordered_map<int, SymptomLink> SymptomsByProblemMap;

// all links connected with a certain symptom, organized by problem ID
typedef boost::unordered_map<int, SymptomLink> ProblemsBySymptomMap;

// all links connected with a certain problem, organized by solution ID
typedef boost::unordered_map<int, SolutionLink> SolutionsByProblemMap;

// all links connected with a certain solution, organized by problem ID
typedef boost::unordered_map<int, SolutionLink> ProblemsBySolutionMap;

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
    virtual void get(const std::vector<int>& categoryIDs, CategoryMap& result, std::vector<int>* notFound = NULL) = 0;
    virtual void get(const std::vector<int>& problemIDs, ProblemMap& result, std::vector<int>* notFound = NULL) = 0;
    virtual void get(const std::vector<int>& symptomIDs, SymptomMap& result, std::vector<int>* notFound = NULL) = 0;
    virtual void get(const std::vector<int>& solutionIDs, SolutionMap& result, std::vector<int>* notFound = NULL) = 0;
    virtual void get(const std::vector<int>& investigationIDs, InvestigationMap& result, std::vector<int>* notFound = NULL) = 0;
    
    /**
     * Get Extended version of objects
     */
    virtual void get(const std::vector<int>& problemIDs, ExtendedProblemMap& result, std::vector<int>* notFound = NULL) = 0;
    virtual void get(const std::vector<int>& symptomIDs, ExtendedSymptomMap& result, std::vector<int>* notFound = NULL) = 0;
    virtual void get(const std::vector<int>& solutionIDs, ExtendedSolutionMap& result, std::vector<int>* notFound = NULL) = 0;
    
    /**
     * The found output value will contain true false depending if the object was found
     * If found is NULL, the function will throw if something is not found
     */
    virtual void getLinksByProblem(int problemID, SymptomsByProblemMap& result, bool* found = NULL) = 0;
    virtual void getLinksBySymptom(int symptomID, ProblemsBySymptomMap& result, bool* found = NULL) = 0;
    
    virtual void getLinksByProblem(int problemID, SolutionsByProblemMap& result, bool* found = NULL) = 0;
    virtual void getLinksBySolution(int solutionID, ProblemsBySolutionMap& result, bool* found = NULL) = 0;
    
};

} // namespace ProblemSolver
