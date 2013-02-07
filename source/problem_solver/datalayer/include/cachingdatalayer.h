/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "datalayer.h"

#include <auto_ptr.h>

namespace ProblemSolver
{

/**
 * A Data layer designed to cache data from a source DataLayer into a cache DataLayer.
 * When accessing, it is first searched in the cache, and if missing, it is looked up in the source
 * and the result is saved into the cache for later use.
 * When modifying, it is saved both into the cache and into the source.
 * The caching datalayer takes ownership of the pointers to source and cache.
 */
class CachingDataLayer: public IDataLayer
{
public:
    
    CachingDataLayer(IDataLayer* source, IDataLayer* cache);
    virtual ~CachingDataLayer(){}

public:
    
    virtual void get(const std::vector<int>& categoryIDs, CategoryMap& result, std::vector<int>* notFound = NULL);
    virtual void get(const std::vector<int>& problemIDs, ProblemMap& result, std::vector<int>* notFound = NULL);
    virtual void get(const std::vector<int>& symptomIDs, SymptomMap& result, std::vector<int>* notFound = NULL);
    virtual void get(const std::vector<int>& solutionIDs, SolutionMap& result, std::vector<int>* notFound = NULL);
    virtual void get(const std::vector<int>& investigationIDs, InvestigationMap& result, std::vector<int>* notFound = NULL);
    
    virtual void get(const std::vector<int>& problemIDs, ExtendedProblemMap& result, std::vector<int>* notFound = NULL);
    virtual void get(const std::vector<int>& symptomIDs, ExtendedSymptomMap& result, std::vector<int>* notFound = NULL);
    virtual void get(const std::vector<int>& solutionIDs, ExtendedSolutionMap& result, std::vector<int>* notFound = NULL);
    
    virtual void getLinksByProblem(int problemID, SymptomsWithSameProblem& result, bool* found = NULL);
    virtual void getLinksBySymptom(int symptomID, ProblemsWithSameSymptom& result, bool* found = NULL);
    
    virtual void getLinksByProblem(int problemID, SolutionsWithSameProblem& result, bool* found = NULL);
    virtual void getLinksBySolution(int solutionID, ProblemsWithSameSolution& result, bool* found = NULL);
    
public:

    virtual int add(const Category& category);
    virtual int add(const ExtendedProblem& problem);
    virtual int add(const ExtendedSymptom& symptom);
    virtual int add(const ExtendedSolution& solution);
    virtual int add(const SymptomLink& symptomLink);
    virtual int add(const SolutionLink& solutionLink);
    virtual int add(const Investigation& solutionLink);

    virtual void modify(const Category& category);
    virtual void modify(const ExtendedProblem& problem);
    virtual void modify(const ExtendedSymptom& symptom);
    virtual void modify(const ExtendedSolution& solution);
    virtual void modify(const SymptomLink& symptomLink);
    virtual void modify(const SolutionLink& solutionLink);
    virtual void modify(const Investigation& solutionLink);

    virtual void remove(const Category& category);
    virtual void remove(const Problem& problem);
    virtual void remove(const Symptom& symptom);
    virtual void remove(const Solution& solution);
    virtual void remove(const SymptomLink& symptomLink);
    virtual void remove(const SolutionLink& solutionLink);
    virtual void remove(const Investigation& solutionLink);
    
private:
    
    template<class T>
    void templateGet(const std::vector<int>& ids, boost::unordered_map<int, T>& result, std::vector<int>* notFound);
    
    template<class T>
    int templateAdd(const T& object);
    
    template<class T>
    void templateModify(const T& object);
    
    template<class T>
    void templateRemove(const T& object);
    
private:
    
    std::auto_ptr<IDataLayer> _source;
    std::auto_ptr<IDataLayer> _cache;
    
};

} // namespace ProblemSolver
