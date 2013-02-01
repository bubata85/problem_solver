/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of FreeBSD license.
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
     * In any of these functions, if id of object is < 0 then a new id will be generated
     */
    virtual Category* modify(const Category* category) = 0;
    virtual Problem* modify(const Problem* problem) = 0;
    virtual Symptom* modify(const Symptom* symptom) = 0;
    virtual Solution* modify(const Solution* solution) = 0;

};

} // namespace ProblemSolver
