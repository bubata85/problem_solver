/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of FreeBSD license.
 */

#pragma once

#include <vector>
#include <stdexcept>

namespace ProblemSolver
{

class Category;
class Problem;
class Symptom;
class Solution;

class SymptomLink;
class SolutionLink;

// Object ID to Object maps
typedef boost::unordered_map<int, Category> CategoryMap;
typedef boost::unordered_map<int, Problem> ProblemMap;
typedef boost::unordered_map<int, Symptom> SymptomMap;
typedef boost::unordered_map<int, Solution> SolutionMap;

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
class DataLayerException: public std::runtime_error
{
public:
    explicit Exception(const std::string& errorMessage):
                        std::runtime_error(errorMessage){}
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
     * The notFound vector will contains the ID's of objects that were not found.
     * If notFound is NULL, the function will throw if something is not found.
     */
    virtual void get(const std::vector<int>& categoryIDs, CategoryMap& result, std::vector<int>* notFound = NULL) = 0;
    virtual void get(const std::vector<int>& problemIDs, ProblemMap& result, std::vector<int>* notFound = NULL) = 0;
    virtual void get(const std::vector<int>& symptomIDs, SymptomMap& result, std::vector<int>* notFound = NULL) = 0;
    virtual void get(const std::vector<int>& solutionIDs, SolutionMap& result, std::vector<int>* notFound = NULL) = 0;
    
    /**
     * The found output value will contain true false depending if the object was found
     * If found is NULL, the function will throw if something is not found
     */
    virtual void getLinksByProblem(int problemID, SymptomsByProblemMap& result, bool* found = NULL) = 0;
    virtual void getLinksBySymptom(int symptomID, ProblemsBySymptomMap& result, bool* found = NULL) = 0;
    
    virtual void getLinksByProblem(int problemID, SolutionsByProblemMap& result, bool* found) = 0;
    virtual void getLinksBySolution(int solutionID, ProblemsBySolutionMap& result, bool* found) = 0;

};

} // namespace ProblemSolver
