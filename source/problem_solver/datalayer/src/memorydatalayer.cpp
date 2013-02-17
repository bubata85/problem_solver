/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "memorydatalayer.h"

namespace ProblemSolver
{

void MemoryDataLayer::get(const std::vector<Identifier>& categoryIDs, CategoryMap& result, std::vector<Identifier>* notFound)
{
    
}
void MemoryDataLayer::get(const std::vector<Identifier>& problemIDs, ProblemMap& result, std::vector<Identifier>* notFound)
{
    
}
void MemoryDataLayer::get(const std::vector<Identifier>& symptomIDs, SymptomMap& result, std::vector<Identifier>* notFound)
{
    
}
void MemoryDataLayer::get(const std::vector<Identifier>& solutionIDs, SolutionMap& result, std::vector<Identifier>* notFound)
{
    
}
void MemoryDataLayer::get(const std::vector<Identifier>& investigationIDs, InvestigationMap& result, std::vector<Identifier>* notFound)
{
    
}
    
void MemoryDataLayer::get(const std::vector<Identifier>& problemIDs, ExtendedProblemMap& result, std::vector<Identifier>* notFound)
{
    
}
void MemoryDataLayer::get(const std::vector<Identifier>& symptomIDs, ExtendedSymptomMap& result, std::vector<Identifier>* notFound)
{
    
}
void MemoryDataLayer::get(const std::vector<Identifier>& solutionIDs, ExtendedSolutionMap& result, std::vector<Identifier>* notFound)
{
    
}
    
void MemoryDataLayer::getLinksByProblem(Identifier problemID, SymptomsWithSameProblem& result, bool* found)
{
    
}
void MemoryDataLayer::getLinksBySymptom(Identifier symptomID, ProblemsWithSameSymptom& result, bool* found)
{
    
}
    
void MemoryDataLayer::getLinksByProblem(Identifier problemID, SolutionsWithSameProblem& result, bool* found)
{
    
}
void MemoryDataLayer::getLinksBySolution(Identifier solutionID, ProblemsWithSameSolution& result, bool* found)
{
    
}

Identifier MemoryDataLayer::add(const Category& category)
{
    return "";
}
Identifier MemoryDataLayer::add(const ExtendedProblem& problem)
{
    return "";
}
Identifier MemoryDataLayer::add(const ExtendedSymptom& symptom)
{
    return "";
}
Identifier MemoryDataLayer::add(const ExtendedSolution& solution)
{
    return "";
}
Identifier MemoryDataLayer::add(const SymptomLink& symptomLink)
{
    return "";
}
Identifier MemoryDataLayer::add(const SolutionLink& solutionLink)
{
    return "";
}
Identifier MemoryDataLayer::add(const Investigation& solutionLink)
{
    return "";
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
