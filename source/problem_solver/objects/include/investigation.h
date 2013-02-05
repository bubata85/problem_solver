/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include <vector>

namespace ProblemSolver
{

/**
 * Holds information about one investigation that has been performed.
 * It is related to some problem we need to identify and/or fix
 */
struct Investigation
{
    int id; // id in the database
    
    // keep in mind you could have a solution before identifying the problem!
    int positiveProblem; // if this is >= 0 it will mean the problem causing the symptoms has been identified
    int positiveSolution; // if this is >= 0 it will mean the solution that fixes the problem has been identified
    
    std::vector<int> positiveSymptoms; // symptoms that have been verified as positive
    std::vector<int> negativeSymptoms; // symptoms that have been verified as negative
    std::vector<int> bannedSymptoms; // symptoms that the user did not wish to verify
    
    std::vector<int> negativeProblems; // problems that have been verified as negative
    std::vector<int> bannedProblems; // problems that the user did not wish to verify
    
    std::vector<int> negativeSolutions; // solutions that have been verified as non-working
    std::vector<int> bannedSolutions; // solutions that the user did not wish to verify
    
    Investigation():id(-1),positiveProblem(-1),positiveSolution(-1){}
};

} // namespace ProblemSolver
