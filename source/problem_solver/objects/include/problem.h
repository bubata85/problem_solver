/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "genericinfo.h"

namespace ProblemSolver
{

/**
 * A problem can be anything that can cause SYMPTOMS, has SOLUTIONS and can be VERIFIED before applying
 * any solution and is likely to fix any symptoms connected with it. Problems are the main structure
 * in the system and problem solving revolves explicitly around them.
 * 
 * There is a very thin line between problems and solutions that needs to be cared for.
 * Should you have 1 more generic problem that has 5 different solutions or 5 more specific problems
 * each with its own solution? It is really up to the users and administrators to define this depth.
 * 
 * One good rule to follow is: If something can be verified, it is a problem, not a solution.
 * If this is done correctly then no problem will ever be added wrongfully as a solution instead.
 * 
 * A good practice is to have only 1 solution for each problem or at most 2, one being a "quick fix"
 * and the other the "long term fix". In theory this is not always possible and this is the reason why the system
 * has no such restriction. In practice however, there are many cases where problems have definitive solutions.
 * If a restriction on the solutions is used it can help keep the knowledge base in good condition by
 * ensuring all problems are as definitive as possible and preventing users from attaching solutions to
 * existing problems instead of creating new ones out of laziness.
 * 
 * It is also a good practice to break the problems down as much as the symptoms allow. If a new symptom
 * suddenly appears that always requires a certain solution out of many, it should be considered splitting
 * the problem in two. If there are many solutions to a problem and there are symptoms that can help pin down
 * the right solution, it is advised to break down the problem in as many specific problems as possible.
 * This will allow the system to use the symptoms to help users narrow down the possible problems/solutions.
 * 
 * If on the other hand there are no symptoms that can distinguish which solution will help resolve a problem,
 * then there is no real gain in further splitting the problem. The system will ultimately advise the user
 * to try each problem/solution only ordering by simple priority. It will not be possible to further
 * narrow the search down.
 */
struct Problem: public GenericInfo
{
};

/**
 * Same as the problem but also contains extended information for use by the user
 */
struct ExtendedProblem: public Problem, public ExtendedGenericInfo
{
};

} // namespace ProblemSolver
