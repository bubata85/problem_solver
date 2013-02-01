/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of FreeBSD license.
 */

#include <stdio.h>

#include "problem.h"
#include "symptom.h"
#include "solution.h"

int main(int argc, const char* argv[])
{
    ProblemSolver::Problem problem;
    ProblemSolver::Symptom symptom;
    ProblemSolver::Solution solution;
    
    printf("Hello world!\n");
    return 0;
}
