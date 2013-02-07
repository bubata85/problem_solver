/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "memorydatalayer.h"

namespace ProblemSolver
{

void MemoryDataLayer::get(const std::vector<int>& categoryIDs, CategoryMap& result, std::vector<int>* notFound)
{
    
}
void MemoryDataLayer::get(const std::vector<int>& problemIDs, ProblemMap& result, std::vector<int>* notFound)
{
    
}
void MemoryDataLayer::get(const std::vector<int>& symptomIDs, SymptomMap& result, std::vector<int>* notFound)
{
    
}
void MemoryDataLayer::get(const std::vector<int>& solutionIDs, SolutionMap& result, std::vector<int>* notFound)
{
    
}
void MemoryDataLayer::get(const std::vector<int>& investigationIDs, InvestigationMap& result, std::vector<int>* notFound)
{
    
}
    
void MemoryDataLayer::get(const std::vector<int>& problemIDs, ExtendedProblemMap& result, std::vector<int>* notFound)
{
    
}
void MemoryDataLayer::get(const std::vector<int>& symptomIDs, ExtendedSymptomMap& result, std::vector<int>* notFound)
{
    
}
void MemoryDataLayer::get(const std::vector<int>& solutionIDs, ExtendedSolutionMap& result, std::vector<int>* notFound)
{
    
}
    
void MemoryDataLayer::getLinksByProblem(int problemID, SymptomsWithSameProblem& result, bool* found)
{
    
}
void MemoryDataLayer::getLinksBySymptom(int symptomID, ProblemsWithSameSymptom& result, bool* found)
{
    
}
    
void MemoryDataLayer::getLinksByProblem(int problemID, SolutionsWithSameProblem& result, bool* found)
{
    
}
void MemoryDataLayer::getLinksBySolution(int solutionID, ProblemsWithSameSolution& result, bool* found)
{
    
}

int MemoryDataLayer::add(const Category& category)
{
    return 0;
}
int MemoryDataLayer::add(const ExtendedProblem& problem)
{
    return 0;
}
int MemoryDataLayer::add(const ExtendedSymptom& symptom)
{
    return 0;
}
int MemoryDataLayer::add(const ExtendedSolution& solution)
{
    return 0;
}
int MemoryDataLayer::add(const SymptomLink& symptomLink)
{
    return 0;
}
int MemoryDataLayer::add(const SolutionLink& solutionLink)
{
    return 0;
}
int MemoryDataLayer::add(const Investigation& solutionLink)
{
    return 0;
}

void MemoryDataLayer::modify(const Category& category)
{
    
}
void MemoryDataLayer::modify(const ExtendedProblem& problem)
{
    
}
void MemoryDataLayer::modify(const ExtendedSymptom& symptom)
{
    
}
void MemoryDataLayer::modify(const ExtendedSolution& solution)
{
    
}
void MemoryDataLayer::modify(const SymptomLink& symptomLink)
{
    
}
void MemoryDataLayer::modify(const SolutionLink& solutionLink)
{
    
}
void MemoryDataLayer::modify(const Investigation& solutionLink)
{
    
}

void MemoryDataLayer::remove(const Category& category)
{
    
}
void MemoryDataLayer::remove(const Problem& problem)
{
    
}
void MemoryDataLayer::remove(const Symptom& symptom)
{
    
}
void MemoryDataLayer::remove(const Solution& solution)
{
    
}
void MemoryDataLayer::remove(const SymptomLink& symptomLink)
{
    
}
void MemoryDataLayer::remove(const SolutionLink& solutionLink)
{
    
}
void MemoryDataLayer::remove(const Investigation& solutionLink)
{
    
}

} // namespace ProblemSolver
