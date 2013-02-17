/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "mysqldatalayer.h"

namespace ProblemSolver
{

void MySqlDataLayer::get(const std::vector<Identifier>& categoryIDs, CategoryMap& result, std::vector<Identifier>* notFound)
{
    
}
void MySqlDataLayer::get(const std::vector<Identifier>& problemIDs, ProblemMap& result, std::vector<Identifier>* notFound)
{
    
}
void MySqlDataLayer::get(const std::vector<Identifier>& symptomIDs, SymptomMap& result, std::vector<Identifier>* notFound)
{
    
}
void MySqlDataLayer::get(const std::vector<Identifier>& solutionIDs, SolutionMap& result, std::vector<Identifier>* notFound)
{
    
}
void MySqlDataLayer::get(const std::vector<Identifier>& investigationIDs, InvestigationMap& result, std::vector<Identifier>* notFound)
{
    
}
    
void MySqlDataLayer::get(const std::vector<Identifier>& problemIDs, ExtendedProblemMap& result, std::vector<Identifier>* notFound)
{
    
}
void MySqlDataLayer::get(const std::vector<Identifier>& symptomIDs, ExtendedSymptomMap& result, std::vector<Identifier>* notFound)
{
    
}
void MySqlDataLayer::get(const std::vector<Identifier>& solutionIDs, ExtendedSolutionMap& result, std::vector<Identifier>* notFound)
{
    
}
    
void MySqlDataLayer::getLinksByProblem(Identifier problemID, SymptomsWithSameProblem& result, bool* found)
{
    
}
void MySqlDataLayer::getLinksBySymptom(Identifier symptomID, ProblemsWithSameSymptom& result, bool* found)
{
    
}
    
void MySqlDataLayer::getLinksByProblem(Identifier problemID, SolutionsWithSameProblem& result, bool* found)
{
    
}
void MySqlDataLayer::getLinksBySolution(Identifier solutionID, ProblemsWithSameSolution& result, bool* found)
{
    
}

Identifier MySqlDataLayer::add(const Category& category)
{
    return "";
}
Identifier MySqlDataLayer::add(const ExtendedProblem& problem)
{
    return "";
}
Identifier MySqlDataLayer::add(const ExtendedSymptom& symptom)
{
    return "";
}
Identifier MySqlDataLayer::add(const ExtendedSolution& solution)
{
    return "";
}
Identifier MySqlDataLayer::add(const SymptomLink& symptomLink)
{
    return "";
}
Identifier MySqlDataLayer::add(const SolutionLink& solutionLink)
{
    return "";
}
Identifier MySqlDataLayer::add(const Investigation& solutionLink)
{
    return "";
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
