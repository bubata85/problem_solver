/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "jsonserialization.h"


namespace ProblemSolver
{

std::string JsonSerializer::serialize(const Category& category)
{
    _result.clear();

    startObject();
    
    addKeyValue("id", category.id);
    addKeyValue("name", category.name);
    addKeyValue("parent", category.parent);
    addKeyValue("description", category.description);
    addArray("childCategories", category.childCategories, false);
    
    endObject();
    
    return _result;
}

std::string JsonSerializer::serialize(const ExtendedSymptom& symptom)
{
    _result.clear();
    addGenericInfo(symptom);
    return _result;
}

std::string JsonSerializer::serialize(const ExtendedProblem& problem)
{
    _result.clear();
    addGenericInfo(problem);
    return _result;
}

std::string JsonSerializer::serialize(const ExtendedSolution& solution)
{
    _result.clear();
    addGenericInfo(solution);
    return _result;
}

std::string JsonSerializer::serialize(const SymptomLink& symptomLink)
{
    _result.clear();

    addKeyValue("id", symptomLink.id);
    addKeyValue("problemID", symptomLink.problemID);
    addKeyValue("symptomID", symptomLink.symptomID);
    addKeyValue("positiveChecks", symptomLink.positiveChecks);
    addKeyValue("falsePositiveChecks", symptomLink.falsePositiveChecks);
    addKeyValue("negativeChecks", symptomLink.negativeChecks);
    addKeyValue("confirmed", symptomLink.confirmed, false);
    
    return _result;
}

std::string JsonSerializer::serialize(const SolutionLink& solutionLink)
{
    _result.clear();

    addKeyValue("id", solutionLink.id);
    addKeyValue("problemID", solutionLink.problemID);
    addKeyValue("solutionID", solutionLink.solutionID);
    addKeyValue("positive", solutionLink.positive);
    addKeyValue("negative", solutionLink.negative);
    addKeyValue("confirmed", solutionLink.confirmed, false);
    
    return _result;
}
    
std::string JsonSerializer::serialize(const Investigation& investigation)
{
    _result.clear();

    addKeyValue("positiveProblem", investigation.positiveProblem);
    addKeyValue("positiveSolution", investigation.positiveSolution);
    
    addArray("positiveSymptoms", investigation.positiveSymptoms);
    addArray("negativeSymptoms", investigation.negativeSymptoms);
    addArray("bannedSymptoms", investigation.bannedSymptoms);
    
    addArray("negativeProblems", investigation.negativeProblems);
    addArray("bannedProblems", investigation.bannedProblems);
    
    addArray("negativeSolutions", investigation.negativeSolutions);
    addArray("bannedSolutions", investigation.bannedSolutions);
    
    return _result;
}


void JsonSerializer::startObject()
{
    _result += "{";
}

void JsonSerializer::endObject()
{
    _result += "}";
}

template<class T>
void JsonSerializer::addGenericInfo(const T& object)
{
    startObject();
    
    addKeyValue("id", object.id);
    addKeyValue("difficulty", object.difficulty);
    addKeyValue("confirmed", object.confirmed);
    addKeyValue("categoryID", object.categoryID);
    addKeyValue("name", object.name);
    addKeyValue("description", object.description);
    addArray("tags", object.tags);
    addArray("steps", object.steps, false);
    
    endObject();
}


Category JsonDeserializer::deserializeCategory(const boost::property_tree::ptree& json)
{
    Category result;
    
    getValue(result.id, "id", json);
    getValue(result.name, "name", json);
    getValue(result.description, "description", json);
    getValue(result.parent, "parent", json);
    getArray(result.childCategories, "childCategories", json);
    
    return result;
}

ExtendedSymptom JsonDeserializer::deserializeSymptom(const boost::property_tree::ptree& json)
{
    ExtendedSymptom result;
    getGenericInfo(result, json);
    return result;
}

ExtendedProblem JsonDeserializer::deserializeProblem(const boost::property_tree::ptree& json)
{
    ExtendedProblem result;
    getGenericInfo(result, json);
    return result;
}

ExtendedSolution JsonDeserializer::deserializeSolution(const boost::property_tree::ptree& json)
{
    ExtendedSolution result;
    getGenericInfo(result, json);
    return result;
}

SymptomLink JsonDeserializer::deserializeSymptomLink(const boost::property_tree::ptree& json)
{
    SymptomLink result;
    
    getValue(result.id, "id", json);
    getValue(result.problemID, "problemID", json);
    getValue(result.symptomID, "symptomID", json);
    getValue(result.positiveChecks, "positiveChecks", json);
    getValue(result.falsePositiveChecks, "falsePositiveChecks", json);
    getValue(result.negativeChecks, "negativeChecks", json);
    getValue(result.confirmed, "confirmed", json);
    
    return result;
}

SolutionLink JsonDeserializer::deserializeSolutionLink(const boost::property_tree::ptree& json)
{
    SolutionLink result;
    
    getValue(result.id, "id", json);
    getValue(result.problemID, "problemID", json);
    getValue(result.solutionID, "solutionID", json);
    getValue(result.positive, "positive", json);
    getValue(result.negative, "negative", json);
    getValue(result.confirmed, "confirmed", json);
    
    return result;
}

Investigation JsonDeserializer::deserializeInvestigation(const boost::property_tree::ptree& json)
{
    Investigation result;
    
    getValue(result.id, "id", json);
    getValue(result.positiveProblem, "positiveProblem", json);
    getValue(result.positiveSolution, "positiveSolution", json);
    
    getArray(result.positiveSymptoms, "positiveSymptoms", json);
    getArray(result.negativeSymptoms, "negativeSymptoms", json);
    getArray(result.bannedSymptoms, "bannedSymptoms", json);
    
    getArray(result.negativeProblems, "negativeProblems", json);
    getArray(result.bannedProblems, "bannedProblems", json);
    
    getArray(result.negativeSolutions, "negativeSolutions", json);
    getArray(result.bannedSolutions, "bannedSolutions", json);
    
    return result;
}

template<class T>
void JsonDeserializer::getGenericInfo(T& object, const boost::property_tree::ptree& json)
{
    getValue(object.id, "id", json);
    int difficulty = 0;
    getValue(difficulty, "difficulty", json);
    if(difficulty < difficultyUnknown || difficulty > difficultyCategoryExpertOnly)
        throw(std::runtime_error("JSON: Cannot deserialize difficulty "));
    object.difficulty = static_cast<DifficultyLevel>(difficulty);
    getValue(object.confirmed, "confirmed", json);
    getValue(object.categoryID, "categoryID", json);
    getValue(object.name, "name", json);
    getValue(object.description, "description", json);
    getArray(object.tags, "tags", json);
    getArray(object.steps, "steps", json);
}

} // namespace ProblemSolver
