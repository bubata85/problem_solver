/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "cachingdatalayer.h"

namespace ProblemSolver
{

CachingDataLayer::CachingDataLayer(IDataLayer* source, IDataLayer* cache):
    _source(source),
    _cache(cache)
{
    // initial load of all related objects
    
}

void CachingDataLayer::get(const std::vector<int>& categoryIDs, CategoryMap& result, std::vector<int>* notFound)
{
    
}
void CachingDataLayer::get(const std::vector<int>& problemIDs, ProblemMap& result, std::vector<int>* notFound)
{
    
}
void CachingDataLayer::get(const std::vector<int>& symptomIDs, SymptomMap& result, std::vector<int>* notFound)
{
    
}
void CachingDataLayer::get(const std::vector<int>& solutionIDs, SolutionMap& result, std::vector<int>* notFound)
{
    
}
void CachingDataLayer::get(const std::vector<int>& investigationIDs, InvestigationMap& result, std::vector<int>* notFound)
{
    
}
    
void CachingDataLayer::get(const std::vector<int>& problemIDs, ExtendedProblemMap& result, std::vector<int>* notFound)
{
    
}
void CachingDataLayer::get(const std::vector<int>& symptomIDs, ExtendedSymptomMap& result, std::vector<int>* notFound)
{
    
}
void CachingDataLayer::get(const std::vector<int>& solutionIDs, ExtendedSolutionMap& result, std::vector<int>* notFound)
{
    
}
    
void CachingDataLayer::getLinksByProblem(int problemID, SymptomsWithSameProblem& result, bool* found)
{
    
}
void CachingDataLayer::getLinksBySymptom(int symptomID, ProblemsWithSameSymptom& result, bool* found)
{
    
}
    
void CachingDataLayer::getLinksByProblem(int problemID, SolutionsWithSameProblem& result, bool* found)
{
    
}
void CachingDataLayer::getLinksBySolution(int solutionID, ProblemsWithSameSolution& result, bool* found)
{
    
}

int CachingDataLayer::add(const Category& category)
{
    return 0;
}
int CachingDataLayer::add(const ExtendedProblem& problem)
{
    return 0;
}
int CachingDataLayer::add(const ExtendedSymptom& symptom)
{
    return 0;
}
int CachingDataLayer::add(const ExtendedSolution& solution)
{
    return 0;
}
int CachingDataLayer::add(const SymptomLink& symptomLink)
{
    return 0;
}
int CachingDataLayer::add(const SolutionLink& solutionLink)
{
    return 0;
}
int CachingDataLayer::add(const Investigation& solutionLink)
{
    return 0;
}

void CachingDataLayer::modify(const Category& category)
{
    
}
void CachingDataLayer::modify(const ExtendedProblem& problem)
{
    
}
void CachingDataLayer::modify(const ExtendedSymptom& symptom)
{
    
}
void CachingDataLayer::modify(const ExtendedSolution& solution)
{
    
}
void CachingDataLayer::modify(const SymptomLink& symptomLink)
{
    
}
void CachingDataLayer::modify(const SolutionLink& solutionLink)
{
    
}
void CachingDataLayer::modify(const Investigation& solutionLink)
{
    
}

void CachingDataLayer::remove(const Category& category)
{
    
}
void CachingDataLayer::remove(const Problem& problem)
{
    
}
void CachingDataLayer::remove(const Symptom& symptom)
{
    
}
void CachingDataLayer::remove(const Solution& solution)
{
    
}
void CachingDataLayer::remove(const SymptomLink& symptomLink)
{
    
}
void CachingDataLayer::remove(const SolutionLink& solutionLink)
{
    
}
void CachingDataLayer::remove(const Investigation& solutionLink)
{
    
}

/**
 * Retrieves objects from the cache, and if they are not found searches for them in the source.
 * Anything found in the source is saved in cache for later use.
 *//*
template<class T>
bool CachingDataLayer::templateGet(const std::vector<int>& ids, std::vector<const T*>& result)
{
    // look up what we can from cache
    if(!_cache->get(ids, result))
        return false;
    
    std::vector<int> cacheMissIDs;
    std::vector<int> cacheMissOriginalIdexes;
    for(uint i = 0; i < result.size(); ++i)
    {
        if(result[i] == NULL)
        {
            cacheMissIDs.push_back(ids[i]);
            cacheMissOriginalIdexes.push_back(i);
        }
    }
    
    // check if we have cache misses
    if(!cacheMissIDs.empty())
    {
        std::vector<const T*> cacheMissObjects;
        cacheMissObjects.reserve(cacheMissIDs.size());
        
        // look up cache misses from source
        if(!_source->get(cacheMissIDs, cacheMissObjects))
            return false;
        
        //  save anything found back to cache and update the result
        for(uint i = 0; i < cacheMissObjects.size(); ++i)
        {
            if(cacheMissObjects[i] != NULL)
            {
                T* cacheUpdate = _cache->modify(cacheMissObjects[i]);
                if(cacheUpdate == NULL)
                    return false;
                
                result[cacheMissOriginalIdexes[i]] = cacheUpdate;
            }
        }
    }
    
    return true;
}

template<class T>
T* CachingDataLayer::templateModify(const T* object)
{
    // update the object inside the source
    T* modifyResult = _source->modify(object);
    if(modifyResult == NULL)
        return NULL;
    
    return _cache->modify(object);
}*/


} // namespace ProblemSolver
