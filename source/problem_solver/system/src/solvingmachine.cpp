/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "solvingmachine.h"

#include "datalayerread.h"

namespace ProblemSolver
{

/**
 * Returns a suggested continue path for identifying the input unknown problem.
 */
SolvingMachine::Suggestion SolvingMachine::makeSuggestion(const Investigation& investigation, IDataLayerRead& dataLayer)
{
    Suggestion suggestion;
    
    
    return suggestion;
}
    
} // namespace ProblemSolver
