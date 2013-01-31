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
     * In any of these functions, if id is < 0 then a new object will be added to the knowledge base
     */
    virtual bool modifyCategory(Category* category) = 0;
    virtual bool modifyProblem(Problem* category) = 0;
    virtual bool modifySymptom(Symptom* category) = 0;
    virtual bool modifySolution(Solution* category) = 0;

};

} // namespace ProblemSolver
