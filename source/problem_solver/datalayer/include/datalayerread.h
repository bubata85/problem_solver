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
    
    virtual bool get(const std::vector<int>& categoryIDs, std::vector<const Category*>& result) = 0;
    virtual bool get(const std::vector<int>& problemIDs, std::vector<const Problem*>& result) = 0;
    virtual bool get(const std::vector<int>& symptomIDs, std::vector<const Symptom*>& result) = 0;
    virtual bool get(const std::vector<int>& solutionIDs, std::vector<const Solution*>& result) = 0;

};

} // namespace ProblemSolver
