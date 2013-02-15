/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "mongodbdatalayer.h"

namespace ProblemSolver
{

void MongoDbDataLayer::get(const std::vector<int>& categoryIDs, CategoryMap& result, std::vector<int>* notFound)
{
    
}
void MongoDbDataLayer::get(const std::vector<int>& problemIDs, ProblemMap& result, std::vector<int>* notFound)
{
    
}
void MongoDbDataLayer::get(const std::vector<int>& symptomIDs, SymptomMap& result, std::vector<int>* notFound)
{
    
}
void MongoDbDataLayer::get(const std::vector<int>& solutionIDs, SolutionMap& result, std::vector<int>* notFound)
{
    
}
void MongoDbDataLayer::get(const std::vector<int>& investigationIDs, InvestigationMap& result, std::vector<int>* notFound)
{
    
}
    
void MongoDbDataLayer::get(const std::vector<int>& problemIDs, ExtendedProblemMap& result, std::vector<int>* notFound)
{
    
}
void MongoDbDataLayer::get(const std::vector<int>& symptomIDs, ExtendedSymptomMap& result, std::vector<int>* notFound)
{
    
}
void MongoDbDataLayer::get(const std::vector<int>& solutionIDs, ExtendedSolutionMap& result, std::vector<int>* notFound)
{
    
}
    
void MongoDbDataLayer::getLinksByProblem(int problemID, SymptomsWithSameProblem& result, bool* found)
{
    
}
void MongoDbDataLayer::getLinksBySymptom(int symptomID, ProblemsWithSameSymptom& result, bool* found)
{
    
}
    
void MongoDbDataLayer::getLinksByProblem(int problemID, SolutionsWithSameProblem& result, bool* found)
{
    
}
void MongoDbDataLayer::getLinksBySolution(int solutionID, ProblemsWithSameSolution& result, bool* found)
{
    
}

int MongoDbDataLayer::add(const Category& category)
{
    return 0;
}
int MongoDbDataLayer::add(const ExtendedProblem& problem)
{
    return 0;
}
int MongoDbDataLayer::add(const ExtendedSymptom& symptom)
{
    return 0;
}
int MongoDbDataLayer::add(const ExtendedSolution& solution)
{
    return 0;
}
int MongoDbDataLayer::add(const SymptomLink& symptomLink)
{
    return 0;
}
int MongoDbDataLayer::add(const SolutionLink& solutionLink)
{
    return 0;
}
int MongoDbDataLayer::add(const Investigation& solutionLink)
{
    return 0;
}

void MongoDbDataLayer::modify(const Category& category)
{
    
}
void MongoDbDataLayer::modify(const ExtendedProblem& problem)
{
    
}
void MongoDbDataLayer::modify(const ExtendedSymptom& symptom)
{
    
}
void MongoDbDataLayer::modify(const ExtendedSolution& solution)
{
    
}
void MongoDbDataLayer::modify(const SymptomLink& symptomLink)
{
    
}
void MongoDbDataLayer::modify(const SolutionLink& solutionLink)
{
    
}
void MongoDbDataLayer::modify(const Investigation& solutionLink)
{
    
}

void MongoDbDataLayer::remove(const Category& category)
{
    
}
void MongoDbDataLayer::remove(const Problem& problem)
{
    
}
void MongoDbDataLayer::remove(const Symptom& symptom)
{
    
}
void MongoDbDataLayer::remove(const Solution& solution)
{
    
}
void MongoDbDataLayer::remove(const SymptomLink& symptomLink)
{
    
}
void MongoDbDataLayer::remove(const SolutionLink& solutionLink)
{
    
}
void MongoDbDataLayer::remove(const Investigation& solutionLink)
{
    
}

} // namespace ProblemSolver
