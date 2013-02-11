/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "datalayer.h"

#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

namespace ProblemSolver
{

class JsonSerialize
{
public:
    
    JsonSerialize();
    ~JsonSerialize(){}
    
public:
    
    const std::string& finish();

public:
    
    void addSymptom(const ExtendedSymptom& symptom);

private:
    
    void startObject();
    void endObject();
    void addKeyValue(const std::string& key, const std::string& value, bool withComma = true);
    
    template<class T>
    void addGenericInfo(const T& object);
    
private:
    
    template <typename T>
    void addKeyValue(const std::string& key, const T& value, bool withComma = true)
    {
        _result += "\"";
        _result += key;
        _result += "\":";
        _result += boost::lexical_cast<std::string>(value);
        if (withComma)
            _result += ",";
    }
    
    template <typename T>
    void addArray(const std::string& key, const T& array, bool withComma = true)
    {
        _result += "\"";
        _result += key;
        _result += "\":[";
        
        iterate(array);

        _result += "]";
        
        if (withComma)
            _result += ",";
    }
    
    template <typename T>
    void iterate(const std::vector<T>& container)
    {
        bool first = true;
        BOOST_FOREACH(const T& value, container)
        {
            _result += first?"\"":"\",";
            _result += boost::lexical_cast<std::string>(value);
            first = false;
        }
        if(!container.empty())
            _result += "\"";
    }
    
    template <typename T>
    void iterate(const boost::unordered_set<T>& container)
    {
        bool first = true;
        BOOST_FOREACH(const T& value, container)
        {
            _result += first?"\"":"\",";
            _result += boost::lexical_cast<std::string>(value);
            first = false;
        }
        if(!container.empty())
            _result += "\"";
    }
    
private:
    
    std::string _result;
    
};

} // namespace ProblemSolver
