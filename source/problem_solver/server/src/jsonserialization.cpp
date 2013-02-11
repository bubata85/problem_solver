/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "jsonserialization.h"


namespace ProblemSolver
{

JsonSerialize::JsonSerialize()
{
    startObject();
}

const std::string& JsonSerialize::finish()
{
    endObject();
    return _result;
}

void JsonSerialize::addSymptom(const ExtendedSymptom& symptom)
{
    addGenericInfo(symptom);
}

void JsonSerialize::startObject()
{
    _result += "{";
}

void JsonSerialize::endObject()
{
    _result += "}";
}

template<class T>
void JsonSerialize::addGenericInfo(const T& object)
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

} // namespace ProblemSolver
