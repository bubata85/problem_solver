/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of FreeBSD license.
 */

#pragma once

#include "difficulty.h"

#include <string>
#include <vector>
#include <boost/unordered_set.hpp>

namespace ProblemSolver
{

/**
 * Holds simple generic information present in many objects used by the system
 */
struct GenericInfo
{
    int id; // id of the object
    DifficultyLevel difficulty; // difficulty level of "completion"
    bool confirmed; // indicates if a specialist has confirmed this object
};

/**
 * Holds extended generic information that is generally needed only by the user
 */
struct ExtendedGenericInfo
{
    int categoryID; // id of the category the object is part of
    
    std::string name; // name of the object
    std::string description; // description of the object
    boost::unordered_set<std::string> tags; // tags associated with the object
    
    std::vector<std::string> steps; // steps needed to "complete"
};

} // namespace ProblemSolver
