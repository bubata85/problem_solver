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
    
    virtual void get(const std::vector<Identifier>& categoryIDs, CategoryMap& result, std::vector<Identifier>* notFound = NULL);
    virtual void get(const std::vector<Identifier>& problemIDs, ProblemMap& result, std::vector<Identifier>* notFound = NULL);
    virtual void get(const std::vector<Identifier>& symptomIDs, SymptomMap& result, std::vector<Identifier>* notFound = NULL);
    virtual void get(const std::vector<Identifier>& solutionIDs, SolutionMap& result, std::vector<Identifier>* notFound = NULL);
    virtual void get(const std::vector<Identifier>& investigationIDs, InvestigationMap& result, std::vector<Identifier>* notFound = NULL);
    
    virtual void get(const std::vector<Identifier>& problemIDs, ExtendedProblemMap& result, std::vector<Identifier>* notFound = NULL);
    virtual void get(const std::vector<Identifier>& symptomIDs, ExtendedSymptomMap& result, std::vector<Identifier>* notFound = NULL);
    virtual void get(const std::vector<Identifier>& solutionIDs, ExtendedSolutionMap& result, std::vector<Identifier>* notFound = NULL);
    
    virtual void getLinksByProblem(Identifier problemID, SymptomsWithSameProblem& result, bool* found = NULL);
    virtual void getLinksBySymptom(Identifier symptomID, ProblemsWithSameSymptom& result, bool* found = NULL);
    
    virtual void getLinksByProblem(Identifier problemID, SolutionsWithSameProblem& result, bool* found = NULL);
    virtual void getLinksBySolution(Identifier solutionID, ProblemsWithSameSolution& result, bool* found = NULL);
    
public:

    virtual Identifier add(const Category& category);
    virtual Identifier add(const ExtendedProblem& problem);
    virtual Identifier add(const ExtendedSymptom& symptom);
    virtual Identifier add(const ExtendedSolution& solution);
    virtual Identifier add(const SymptomLink& symptomLink);
    virtual Identifier add(const SolutionLink& solutionLink);
    virtual Identifier add(const Investigation& solutionLink);

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
    void templateGet(const std::vector<Identifier>& ids, boost::unordered_map<Identifier, T>& result, std::vector<Identifier>* notFound);
    
    template<class T>
    Identifier templateAdd(const T& object);
    
    template<class T>
    void templateModify(const T& object);
    
    template<class T>
    void templateRemove(const T& object);
    
private:
    
    std::auto_ptr<IDataLayer> _source;
    std::auto_ptr<IDataLayer> _cache;
    
};

} // namespace ProblemSolver
