/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "datalayerread.h"

namespace ProblemSolver
{

/**
 * Interface for accessing the DataLayer for both read and write operations
 */
class IDataLayer: public IDataLayerRead
{
public:
    
    virtual ~IDataLayer(){}
    
public:
    
    /**
     * In add functions the ID of the object is irrelevant, the ID of the new object is returned
     */
    virtual int add(const Category& category) = 0;
    virtual int add(const ExtendedProblem& problem) = 0;
    virtual int add(const ExtendedSymptom& symptom) = 0;
    virtual int add(const ExtendedSolution& solution) = 0;
    virtual int add(const SymptomLink& symptomLink) = 0;
    virtual int add(const SolutionLink& solutionLink) = 0;
    virtual int add(const Investigation& investigation) = 0;
    
    /**
     * There is no way to edit the object ID, it is always used to find the object needing change
     */
    virtual void modify(const Category& category) = 0;
    virtual void modify(const ExtendedProblem& problem) = 0;
    virtual void modify(const ExtendedSymptom& symptom) = 0;
    virtual void modify(const ExtendedSolution& solution) = 0;
    virtual void modify(const SymptomLink& symptomLink) = 0;
    virtual void modify(const SolutionLink& solutionLink) = 0;
    virtual void modify(const Investigation& investigation) = 0;
    
    /**
     * Only the ID of the supplied object is used
     */
    virtual void remove(const Category& category) = 0;
    virtual void remove(const Problem& problem) = 0;
    virtual void remove(const Symptom& symptom) = 0;
    virtual void remove(const Solution& solution) = 0;
    virtual void remove(const SymptomLink& symptomLink) = 0;
    virtual void remove(const SolutionLink& solutionLink) = 0;
    virtual void remove(const Investigation& investigation) = 0;
    
};

} // namespace ProblemSolver
