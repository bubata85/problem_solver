/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of FreeBSD license.
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
    
    virtual bool get(const std::vector<int>& categoryIDs, std::vector<const Category*>& result);
    virtual bool get(const std::vector<int>& problemIDs, std::vector<const Problem*>& result);
    virtual bool get(const std::vector<int>& symptomIDs, std::vector<const Symptom*>& result);
    virtual bool get(const std::vector<int>& solutionIDs, std::vector<const Solution*>& result);
    
public:

    virtual Category* modify(const Category* category);
    virtual Problem* modify(const Problem* problem);
    virtual Symptom* modify(const Symptom* symptom);
    virtual Solution* modify(const Solution* solution);
    
private:
    
    template<class T>
    bool templateGet(const std::vector<int>& ids, std::vector<const T*>& result);
    
    template<class T>
    T* templateModify(const T* object);
    
private:
    
    std::auto_ptr<IDataLayer> _source;
    std::auto_ptr<IDataLayer> _cache;
    
};

} // namespace ProblemSolver
