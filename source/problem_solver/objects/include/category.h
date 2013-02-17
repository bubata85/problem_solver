/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "identifier.h"

#include <vector>

namespace ProblemSolver
{

class Symptom;
class Problem;
    
/**
 * A category is used to house related symptoms, problems and solutions.
 * It can have a parent Category and many child Categories.
 * This promotes organization among the objects and allows the search to target the whole
 * category branch (everything derived from the category + the path to root)
 */
struct Category
{
    Identifier id;
    std::string name;
    std::string description;
    
    Identifier parent; // if empty this is a root
    std::vector<Identifier> childs; // contains all children of this category
};

} // namespace ProblemSolver
