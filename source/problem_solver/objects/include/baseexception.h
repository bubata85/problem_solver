/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include <stdexcept>

namespace ProblemSolver
{

/**
 * Base exception that is the root of all other exceptions in the project
 */
class BaseException: public std::runtime_error
{
public:
    explicit BaseException(const std::string& errorMessage):
        std::runtime_error(errorMessage){}
        
public:
    
    enum ExceptionCode
    {
        exceptionCodeDataLayer = 0,
        exceptionCodeSystemManager = 1,
        exceptionCodeRemoteJsonManager = 2
    };
    
public:
    
    virtual ExceptionCode getCode() const = 0;
    
};

} // namespace ProblemSolver
