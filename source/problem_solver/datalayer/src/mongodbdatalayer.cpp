/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "mongodbdatalayer.h"

namespace ProblemSolver
{

void MongoDbDataLayer::get(const std::vector<Identifier>& categoryIDs, CategoryMap& result, std::vector<Identifier>* notFound)
{
    
}
void MongoDbDataLayer::get(const std::vector<Identifier>& problemIDs, ProblemMap& result, std::vector<Identifier>* notFound)
{
    
}
void MongoDbDataLayer::get(const std::vector<Identifier>& symptomIDs, SymptomMap& result, std::vector<Identifier>* notFound)
{
    
}
void MongoDbDataLayer::get(const std::vector<Identifier>& solutionIDs, SolutionMap& result, std::vector<Identifier>* notFound)
{
    
}
void MongoDbDataLayer::get(const std::vector<Identifier>& investigationIDs, InvestigationMap& result, std::vector<Identifier>* notFound)
{
    
}
    
void MongoDbDataLayer::get(const std::vector<Identifier>& problemIDs, ExtendedProblemMap& result, std::vector<Identifier>* notFound)
{
    
}
void MongoDbDataLayer::get(const std::vector<Identifier>& symptomIDs, ExtendedSymptomMap& result, std::vector<Identifier>* notFound)
{
    
}
void MongoDbDataLayer::get(const std::vector<Identifier>& solutionIDs, ExtendedSolutionMap& result, std::vector<Identifier>* notFound)
{
    
}
    
void MongoDbDataLayer::getLinksByProblem(Identifier problemID, SymptomsWithSameProblem& result, bool* found)
{
    
}
void MongoDbDataLayer::getLinksBySymptom(Identifier symptomID, ProblemsWithSameSymptom& result, bool* found)
{
    
}
    
void MongoDbDataLayer::getLinksByProblem(Identifier problemID, SolutionsWithSameProblem& result, bool* found)
{
    
}
void MongoDbDataLayer::getLinksBySolution(Identifier solutionID, ProblemsWithSameSolution& result, bool* found)
{
    
}

Identifier MongoDbDataLayer::add(const Category& category)
{
    return "";
}
Identifier MongoDbDataLayer::add(const ExtendedProblem& problem)
{
    return "";
}
Identifier MongoDbDataLayer::add(const ExtendedSymptom& symptom)
{
    return "";
}
Identifier MongoDbDataLayer::add(const ExtendedSolution& solution)
{
    return "";
}
Identifier MongoDbDataLayer::add(const SymptomLink& symptomLink)
{
    return "";
}
Identifier MongoDbDataLayer::add(const SolutionLink& solutionLink)
{
    return "";
}
Identifier MongoDbDataLayer::add(const Investigation& solutionLink)
{
    return "";
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
