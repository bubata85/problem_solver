/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of FreeBSD license.
 */

/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of FreeBSD license.
 */

#pragma once

#include "genericinfo.h"

#include <boost/unordered_map.hpp>

namespace ProblemSolver
{

class Problem;
class SymptomLink;

/**
 * A problem can be anything that can be solved and is likely to fix any symptoms connected with it.
 * Problems are the main structure in the system and problem solving revolves explicitly around them.
 */
struct Symptom: public GenericInfo
{
    // information about the problems this symptom is connected to
    // key is problem ID, value is pointer to link
    typedef boost::unordered_map<int, SymptomLink*> ProblemToLinkMap;
    ProblemToLinkMap problemLinks;
};

/**
 * Holds information about the relation between one problem and one symptom.
 */
struct SymptomLink
{
    int id;
    
    Problem* problem;
    Symptom* symptom;
    
    int positiveChecks; // how many times were the problem and the symptom both positive
    int falsePositiveChecks; // how many times was the problem negative but the symptom positive
    int negativeChecks; // how many times was the problem positive but the symptom negative
    
    bool confirmed; // indicates if a specialist has confirmed this relation
};

} // namespace ProblemSolver
