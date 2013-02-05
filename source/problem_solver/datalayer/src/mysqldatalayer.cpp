/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "mysqldatalayer.h"

namespace ProblemSolver
{

/*
bool CachingDataLayer::get(const std::vector<int>& categoryIDs, std::vector<const Category*>& result)
{
    return templateGet(categoryIDs, result);
}

bool CachingDataLayer::get(const std::vector<int>& problemIDs, std::vector<const Problem*>& result)
{
    return templateGet(problemIDs, result);
}

bool CachingDataLayer::get(const std::vector<int>& symptomIDs, std::vector<const Symptom*>& result)
{
    return templateGet(symptomIDs, result);
}

bool CachingDataLayer::get(const std::vector<int>& solutionIDs, std::vector<const Solution*>& result)
{
    return templateGet(solutionIDs, result);
}

Category* CachingDataLayer::modify(const Category* category)
{
    return templateModify(category);
}

Problem* CachingDataLayer::modify(const Problem* problem)
{
    return templateModify(problem);
}

Symptom* CachingDataLayer::modify(const Symptom* symptom)
{
    return templateModify(symptom);
}

Solution* CachingDataLayer::modify(const Solution* solution)
{
    return templateModify(solution);
}*/

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
