/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

namespace utils
{

template<class T, class Y>
bool contains(const T& container, const Y& element)
{
    for(unsigned int i = 0; i < container.size(); ++i)
    {
        if(container[i] == element)
            return true;
    }
    
    return false;
}

} // namespace utils
