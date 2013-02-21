/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "datalayer.h"
#include "systemmanager.h"
#include "solvingmachine.h"

#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>

namespace ProblemSolver
{

/**
 * Class used to serialize ProblemSolver objects in JSON format
 */
class JsonSerializer
{
public:
    
    JsonSerializer(){}
    ~JsonSerializer(){}

public:
    
    std::string serialize(const Category& category);
    
    std::string serialize(const ExtendedSymptom& symptom);
    std::string serialize(const ExtendedProblem& problem);
    std::string serialize(const ExtendedSolution& solution);
    
    std::string serialize(const SymptomLink& symptomLink);
    std::string serialize(const SolutionLink& solutionLink);
    
    std::string serialize(const Investigation& investigation);
    
    std::string serialize(const SystemManager::SearchResult& searchResult);
    std::string serialize(const SolvingMachine::Suggestion& suggestion);

private:
    
    void startObject();
    void endObject();
    
    template<class T>
    void addGenericInfo(const T& object);
    
private:
    
    void addValue(const std::string& value)
    {
        _result += "\"";
        _result += value;
        _result += "\"";
    }
    
    void addValue(bool value)
    {
        _result += value ? "true" : "false";
    }
    
    template <typename T>
    void addValue(const T& value)
    {
        _result += boost::lexical_cast<std::string>(value);
    }
    
    template <typename T>
    void addKeyValue(const std::string& key, const T& value, bool withComma = true)
    {
        _result += "\"";
        _result += key;
        _result += "\":";
        addValue(value);
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
    void iterate(const T& container)
    {
        bool first = true;
        BOOST_FOREACH(const typename T::value_type& value, container)
        {
            if(!first)
                _result += ",";
            addValue(value);
            first = false;
        }
    }
    
private:
    
    std::string _result;
    
};

class JsonDeserializer
{
public:
    
    JsonDeserializer(){}
    ~JsonDeserializer(){}
    
public:
    
    void deserialize(const boost::property_tree::ptree& json, bool getID, Category& result);
    
    void deserialize(const boost::property_tree::ptree& json, bool getID, ExtendedSymptom& result);
    void deserialize(const boost::property_tree::ptree& json, bool getID, ExtendedProblem& result);
    void deserialize(const boost::property_tree::ptree& json, bool getID, ExtendedSolution& result);

    void deserialize(const boost::property_tree::ptree& json, bool getID, SymptomLink& result);
    void deserialize(const boost::property_tree::ptree& json, bool getID, SolutionLink& result);
    
    void deserialize(const boost::property_tree::ptree& json, bool getID, Investigation& result);

private:
    
    template<class T>
    void getGenericInfo(T& object, bool getID, const boost::property_tree::ptree& json);
    
public:
    
    template<class T>
    void getValue(T& value, const std::string& name, const boost::property_tree::ptree& json)
    {
        value = json.get<T>(name);
    }
    
    template<class T>
    void getArray(std::vector<T>& array, const std::string& name, const boost::property_tree::ptree& json)
    {
        const boost::property_tree::ptree& node = json.get_child(name);
        BOOST_FOREACH(const boost::property_tree::ptree::value_type& value, node)
        {
            array.push_back(value.second.get_value<T>());
        }
    }
    
    template<class T>
    void getArray(boost::unordered_set<T>& array, const std::string& name, const boost::property_tree::ptree& json)
    {
        const boost::property_tree::ptree& node = json.get_child(name);

        BOOST_FOREACH(const boost::property_tree::ptree::value_type& value, node)
        {
            array.insert(value.second.get_value<T>());
        }
    }
    
};

} // namespace ProblemSolver
