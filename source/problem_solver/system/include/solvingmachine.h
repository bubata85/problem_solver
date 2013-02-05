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

class IDataLayerRead;
class Investigation;
    
/**
 * This class groups all functionality connected with making suggestions about how to identify unknown problems
 */
class SolvingMachine
{
public:
    
    /**
     * This is the result of a makeSuggestion call.
     * It provides the system's opinion about what should be made next.
     */
    struct Suggestion
    {
        std::vector<int> symptoms; // list of symptoms that can be checked
        std::vector<int> symptomValues; // numeric representation of how worth it is to check a symptom
        
        std::vector<int> problems; // list of problems that can be checked
        std::vector<int> problemValues; // numeric representation of how worth it is to check a problem
        
        std::vector<int> solutions; // list of solutions that can be applied
        std::vector<int> solutionValues; // numeric representation of how worth it is to apply a solution
    };
    
public:
    
    static Suggestion makeSuggestion(const Investigation& Investigation, IDataLayerRead& dataLayer);

};

} // namespace ProblemSolver
