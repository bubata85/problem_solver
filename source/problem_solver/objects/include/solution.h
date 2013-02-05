/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "genericinfo.h"

#include <boost/unordered_set.hpp>

namespace ProblemSolver
{

class Problem;
class SolutionLink;

/**
 * A solution contains the steps in order to resolve a problem.
 * In some cases more than one problems can have the same solution.
 * Sometimes solutions can be used during problem identification.
 * This is because it is sometimes cheaper to apply a solution instead of verifying
 * symptoms or problems.
 * Take this example: Vending machine.
 * Symptoms - takes cash, makes sounds, does not vend anything
 * Problems - Internal mechanism jam, No supplies, Supply jam
 * If there are supplies, it could not be possible to tell if there is any type of jam.
 * The system will however propose to "kick the machine" as this is a good and quick solution to both
 * jams. This will however only be done if the user has selected "cannot check" for at least one verification.
 */
struct Solution: public GenericInfo
{
};

/**
 * Same as the solution but also contains extended information for use by the user
 */
struct ExtendedSolution: public Solution, public ExtendedGenericInfo
{
};

/**
 * Holds information about the relation between one problem and one solution.
 */
struct SolutionLink
{
    int id;
    
    int problemID;
    int solutionID;
    
    int positive; // how many times the solution fixed the problem
    int negative; // how many times the solution did not fixed the problem
    
    bool confirmed; // indicates if a specialist has confirmed this solution
    
    SolutionLink():id(-1),problemID(-1),solutionID(-1),positive(0),negative(0),confirmed(false){}
};

} // namespace ProblemSolver
