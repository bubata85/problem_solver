/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of FreeBSD license.
 */

#pragma once

#include <vector>

namespace ProblemSolver
{

class Category;
class Problem;
class Symptom;
class Solution;

/**
 * Interface for accessing the DataLayer for read-only operations
 */
class IDataLayerRead
{
public:
    
    virtual ~IDataLayerRead(){}
    
public:
    
    virtual std::vector<Category*> getCategories(std::vector<int> categoryIDs) = 0;
    virtual std::vector<Problem*> getProblems(std::vector<int> problemIDs) = 0;
    virtual std::vector<Symptom*> getSymptoms(std::vector<int> symptomIDs) = 0;
    virtual std::vector<Solution*> getSolutions(std::vector<int> solutionIDs) = 0;
};

} // namespace ProblemSolver
