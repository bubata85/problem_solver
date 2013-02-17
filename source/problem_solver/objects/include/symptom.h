/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "genericinfo.h"

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
};

/**
 * Same as the symptom but also contains extended information for use by the user
 */
struct ExtendedSymptom: public Symptom, public ExtendedGenericInfo
{
};

/**
 * Holds information about the relation between one problem and one symptom.
 */
struct SymptomLink
{
    Identifier id;
    
    Identifier problemID;
    Identifier symptomID;
    
    int positiveChecks; // how many times were the problem and the symptom both positive
    int falsePositiveChecks; // how many times was the problem negative but the symptom positive
    int negativeChecks; // how many times was the problem positive but the symptom negative
    
    bool confirmed; // indicates if a specialist has confirmed this relation
    
    SymptomLink():positiveChecks(0),falsePositiveChecks(0),negativeChecks(0),confirmed(false){}
};

} // namespace ProblemSolver
