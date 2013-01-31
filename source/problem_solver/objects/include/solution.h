/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of FreeBSD license.
 */

#pragma once

#include "genericinfo.h"

#include <boost/unordered_map.hpp>

namespace ProblemSolver
{

class Problem;
class SolutionLink;

/**
 * A solution contains the steps in order to resolve a problem.
 * In some cases more than one problem can have the same solution
 */
struct Solution: public GenericInfo
{
    // information about the problems this solution is connected to
    // key is problem ID, value is pointer to link
    typedef boost::unordered_map<int, SolutionLink*> ProblemToLinkMap;
    ProblemToLinkMap symptomLinks;
};

/**
 * Holds information about the relation between one problem and one solution.
 */
struct SolutionLink
{
    int id;
    
    Problem* problem;
    Solution* solution;
    
    int positive; // how many times the solution fixed the problem
    int negative; // how many times the solution did not fixed the problem
    
    bool confirmed; // indicates if a specialist has confirmed this solution
};

} // namespace ProblemSolver
