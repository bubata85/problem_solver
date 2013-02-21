/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "identifier.h"

#include <vector>

namespace ProblemSolver
{

/**
 * Holds information about one investigation that has been performed.
 * It is related to some problem we need to identify and/or fix
 */
struct Investigation
{
    Identifier id; // id in the database
    bool closed; // whether this investigation is no longer relevant
    
    // keep in mind you could have a solution before identifying the problem!
    Identifier positiveProblem; // if this is empty it will mean the problem causing the symptoms has been identified
    Identifier positiveSolution; // if this is empty it will mean the solution that fixes the problem has been identified
    
    std::vector<Identifier> positiveSymptoms; // symptoms that have been verified as positive
    std::vector<Identifier> negativeSymptoms; // symptoms that have been verified as negative
    std::vector<Identifier> bannedSymptoms; // symptoms that the user did not wish to verify
    
    std::vector<Identifier> negativeProblems; // problems that have been verified as negative
    std::vector<Identifier> bannedProblems; // problems that the user did not wish to verify
    
    std::vector<Identifier> negativeSolutions; // solutions that have been verified as non-working
    std::vector<Identifier> bannedSolutions; // solutions that the user did not wish to verify

    Investigation():closed(false){}
    
    bool operator == (const Investigation& compare) const
    {
        return (id == compare.id &&
                closed == compare.closed &&
                positiveProblem == compare.positiveProblem &&
                positiveSolution == compare.positiveSolution &&
                positiveSymptoms == compare.positiveSymptoms &&
                negativeSymptoms == compare.negativeSymptoms &&
                bannedSymptoms == compare.bannedSymptoms &&
                negativeProblems == compare.negativeProblems &&
                bannedProblems == compare.bannedProblems &&
                negativeSolutions == compare.negativeSolutions &&
                bannedSolutions == compare.bannedSolutions);
    }
};

} // namespace ProblemSolver
