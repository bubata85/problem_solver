/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of FreeBSD license.
 */

#pragma once

#include <vector>

namespace ProblemSolver
{

class Symptom;
class Problem;
    
/**
 * This class groups all functionality connected with making suggestions about how to identify unknown problems
 */
class SolvingMachine
{
public:
    
    /**
     * Holds information about a problem we need to identify
     */
    struct UnknownProblem
    {
        std::vector<Symptom*> positiveSymptoms; // symptoms that have been verified as positive
        std::vector<Symptom*> negativeSymptoms; // symptoms that have been verified as negative
        std::vector<Symptom*> bannedSymptoms; // symptoms that the user does not wish to verify
        
        std::vector<Problem*> negativeProblems; // problems that have been verified as negative
        std::vector<Problem*> bannedProblems; // problems that the user does not wish to verify
    };
    
    /**
     * This is the result of a makeSuggestion call.
     * It provides the system's opinion about what should be made next.
     */
    struct Suggestion
    {
        std::vector<Symptom*> symptoms; // list of symptoms that can be checked
        std::vector<int> symptomValues; // value for checking each symptom
        
        std::vector<Problem*> problems; // list of problems that can be checked
        std::vector<int> problemValues; // value for checking each problem
    };
    
public:
    
    static Suggestion makeSuggestion(const UnknownProblem& unknownProblem);

};

} // namespace ProblemSolver
