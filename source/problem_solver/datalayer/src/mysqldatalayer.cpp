/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "mysqldatalayer.h"

namespace ProblemSolver
{

void MySqlDataLayer::get(const std::vector<int>& categoryIDs, CategoryMap& result, std::vector<int>* notFound)
{
    
}
void MySqlDataLayer::get(const std::vector<int>& problemIDs, ProblemMap& result, std::vector<int>* notFound)
{
    
}
void MySqlDataLayer::get(const std::vector<int>& symptomIDs, SymptomMap& result, std::vector<int>* notFound)
{
    
}
void MySqlDataLayer::get(const std::vector<int>& solutionIDs, SolutionMap& result, std::vector<int>* notFound)
{
    
}
void MySqlDataLayer::get(const std::vector<int>& investigationIDs, InvestigationMap& result, std::vector<int>* notFound)
{
    
}
    
void MySqlDataLayer::get(const std::vector<int>& problemIDs, ExtendedProblemMap& result, std::vector<int>* notFound)
{
    
}
void MySqlDataLayer::get(const std::vector<int>& symptomIDs, ExtendedSymptomMap& result, std::vector<int>* notFound)
{
    
}
void MySqlDataLayer::get(const std::vector<int>& solutionIDs, ExtendedSolutionMap& result, std::vector<int>* notFound)
{
    
}
    
void MySqlDataLayer::getLinksByProblem(int problemID, SymptomsWithSameProblem& result, bool* found)
{
    
}
void MySqlDataLayer::getLinksBySymptom(int symptomID, ProblemsWithSameSymptom& result, bool* found)
{
    
}
    
void MySqlDataLayer::getLinksByProblem(int problemID, SolutionsWithSameProblem& result, bool* found)
{
    
}
void MySqlDataLayer::getLinksBySolution(int solutionID, ProblemsWithSameSolution& result, bool* found)
{
    
}

int MySqlDataLayer::add(const Category& category)
{
    return 0;
}
int MySqlDataLayer::add(const ExtendedProblem& problem)
{
    return 0;
}
int MySqlDataLayer::add(const ExtendedSymptom& symptom)
{
    return 0;
}
int MySqlDataLayer::add(const ExtendedSolution& solution)
{
    return 0;
}
int MySqlDataLayer::add(const SymptomLink& symptomLink)
{
    return 0;
}
int MySqlDataLayer::add(const SolutionLink& solutionLink)
{
    return 0;
}
int MySqlDataLayer::add(const Investigation& solutionLink)
{
    return 0;
}

void MySqlDataLayer::modify(const Category& category)
{
    
}
void MySqlDataLayer::modify(const ExtendedProblem& problem)
{
    
}
void MySqlDataLayer::modify(const ExtendedSymptom& symptom)
{
    
}
void MySqlDataLayer::modify(const ExtendedSolution& solution)
{
    
}
void MySqlDataLayer::modify(const SymptomLink& symptomLink)
{
    
}
void MySqlDataLayer::modify(const SolutionLink& solutionLink)
{
    
}
void MySqlDataLayer::modify(const Investigation& solutionLink)
{
    
}

void MySqlDataLayer::remove(const Category& category)
{
    
}
void MySqlDataLayer::remove(const Problem& problem)
{
    
}
void MySqlDataLayer::remove(const Symptom& symptom)
{
    
}
void MySqlDataLayer::remove(const Solution& solution)
{
    
}
void MySqlDataLayer::remove(const SymptomLink& symptomLink)
{
    
}
void MySqlDataLayer::remove(const SolutionLink& solutionLink)
{
    
}
void MySqlDataLayer::remove(const Investigation& solutionLink)
{
    
}

} // namespace ProblemSolver
