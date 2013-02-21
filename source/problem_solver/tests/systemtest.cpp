/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "mongodbdatalayer.h"

#include "systemmanager.h"
#include "remotejsonmanager.h"
#include "jsonserialization.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace boost::property_tree;
using namespace ProblemSolver;

/**
 * Dummy manager used to send requests from code without TCP connections
 */
class DummyRemoteManager: public RemoteJsonManager
{
public:
    
    DummyRemoteManager(SystemManager& systemManager):RemoteJsonManager(systemManager){}
    ~DummyRemoteManager(){}
    
    std::string testRequest(const std::string request)
    {
        return processRequest(request);
    }
    
};

template<class T>
bool testObject(const T& testObject, const std::string& objectName)
{
    JsonSerializer serializer;
    JsonDeserializer deserializer;
    
    std::string serializedString = serializer.serialize(testObject);
    printf("%s\n", serializedString.c_str());
    std::stringstream jsonStream;
    jsonStream << serializedString;
    ptree jsonTree;
    json_parser::read_json(jsonStream, jsonTree);
    
    T testObjectResult;
    deserializer.deserialize(jsonTree, true, testObjectResult);
    if(!(testObject == testObjectResult))
    {
        printf("Error serialize/deserialize %s!\n", objectName.c_str());
        return false;
    }

    printf("Serialize/deserialize %s OK!\n", objectName.c_str());
    return true;
}

template<class T>
bool testSaveLoadObject(T& testObject, const std::string& objectName, DummyRemoteManager& dummyManager)
{
    JsonSerializer serializer;
    JsonDeserializer deserializer;
    
    std::string serializedString = serializer.serialize(testObject);
    
    std::string query = "\n\n{";
    query += "\"RequestType\" : \"database\", ";
    query += "\"ObjectType\" : \"" + objectName + "\", ";
    query += "\"operation\" : \"add\", ";
    query += "\"object\" : " + serializedString + " }";
    
    std::string response = dummyManager.testRequest(query);
    
    printf("Add %s result: %s\n", objectName.c_str(), response.c_str());
    
    std::stringstream jsonStream;
    jsonStream << response;
    ptree jsonTree;
    json_parser::read_json(jsonStream, jsonTree);
    
    std::string newID = jsonTree.get<std::string>("result");
    
    T newTestObject = testObject;
    newTestObject.id = newID;
    
    std::string serializedStringGet = serializer.serialize(newTestObject);
    
    std::string queryGet = "\n\n{ ";
    queryGet += "\"RequestType\" : \"database\", ";
    queryGet += "\"ObjectType\" : \"" + objectName + "\", ";
    queryGet += "\"operation\" : \"get\", ";
    queryGet += "\"ids\" : [\"" + newID + "\"] }";
    
    std::string responseGet = dummyManager.testRequest(queryGet);
    printf("Get %s result: %s\n", objectName.c_str(), responseGet.c_str());

    std::stringstream jsonStreamGet;
    jsonStreamGet << responseGet;
    ptree jsonTreeGet;
    json_parser::read_json(jsonStreamGet, jsonTreeGet);
    
    const ptree& jsonObject = jsonTreeGet.get_child("result").begin()->second;
    
    T testObjectResult;
    deserializer.deserialize(jsonObject, true, testObjectResult);
    if(!(newTestObject == testObjectResult))
    {
        printf("Error save/load %s!\n", objectName.c_str());
        return false;
    }

    testObject.id = newID;
    
    printf("Save/Load %s OK!\n", objectName.c_str());
    return true;
}

bool testSearch(const std::string& phrase, CIdentifier identifier, DummyRemoteManager& dummyManager)
{
    std::string query = "\n\n{";
    query += "\"RequestType\" : \"search\", ";
    query += "\"search\" : \"" + phrase + "\" }";
    
    std::string response = dummyManager.testRequest(query);
    
    printf("Search '%s' result: %s\n", phrase.c_str(), response.c_str());
    
    std::stringstream jsonStream;
    jsonStream << response;
    ptree jsonTree;
    json_parser::read_json(jsonStream, jsonTree);
    
    ptree node = jsonTree.get_child("symptoms");
    BOOST_FOREACH(const ptree::value_type& value, node)
    {
        if(identifier == value.second.get_value<std::string>())
        {
            printf("Search '%s' OK!\n", phrase.c_str());
            return true;
        }
    }

    printf("Error search '%s'!\n", phrase.c_str());
    return false;
}

int main(int argc, const char* argv[])
{
    Category testCategory;
    testCategory.id = "testCategoryID";
    testCategory.name = "testCategoryName";
    testCategory.description = "testCategoryDescription";
    testCategory.parent = "testCategoryParent";
    testCategory.childs.push_back("testCategoryChild1");
    testCategory.childs.push_back("testCategoryChild2");
    testCategory.childs.push_back("testCategoryChild3");

    ExtendedSymptom testSymptom;
    testSymptom.id = "testID";
    testSymptom.categoryID = "testCategoryID";
    testSymptom.difficulty = difficultyManyTimesAndStillNotEasy;
    testSymptom.confirmed = false;
    testSymptom.name = "testName";
    testSymptom.description = "testDescription";
    testSymptom.tags.insert("testTag1");
    testSymptom.tags.insert("testTag2");
    testSymptom.tags.insert("testTag3");
    testSymptom.steps.push_back("testStep1");
    testSymptom.steps.push_back("testStep1");
    testSymptom.steps.push_back("testStep1");
    
    ExtendedProblem testProblem;
    testProblem.id = "testID123";
    testProblem.categoryID = "testCategoryID";
    testProblem.difficulty = difficultyManyTimesAndStillNotEasy;
    testProblem.confirmed = false;
    testProblem.name = "testNameProblem";
    testProblem.description = "testDescription";
    testProblem.tags.insert("testTag1");
    testProblem.tags.insert("testTag2");
    testProblem.tags.insert("testTag23");
    testProblem.steps.push_back("testStep1");
    testProblem.steps.push_back("testStep2");
    testProblem.steps.push_back("testStep32");
    
    ExtendedSolution testSolution;
    testSolution.id = "testID123111";
    testSolution.categoryID = "testCategoryID";
    testSolution.difficulty = difficultyManyTimesAndStillNotEasy;
    testSolution.confirmed = false;
    testSolution.name = "testNameSolution";
    testSolution.description = "testDescription";
    testSolution.tags.insert("testTag21");
    testSolution.tags.insert("testTag22");
    testSolution.tags.insert("testTag223");
    testSolution.steps.push_back("testStep11");
    testSolution.steps.push_back("testStep12");
    testSolution.steps.push_back("testStep132");
    
    SymptomLink testSymptomLink;
    testSymptomLink.id = "testSymptomLink";
    testSymptomLink.problemID = "testID123";
    testSymptomLink.symptomID = "testID";
    testSymptomLink.positiveChecks = 12;
    testSymptomLink.falsePositiveChecks = 10;
    testSymptomLink.negativeChecks = 5;
    testSymptomLink.confirmed = false;
    
    SolutionLink testSolutionLink;
    testSolutionLink.id = "testSolutionLink";
    testSolutionLink.problemID = "testID123";
    testSolutionLink.solutionID = "testID123111";
    testSolutionLink.positive = 12;
    testSolutionLink.negative = 10;
    testSolutionLink.confirmed = false;
    
    Investigation testInvestigation;
    testInvestigation.id = "testIDinvestigation";
    testInvestigation.closed = true;
    testInvestigation.positiveProblem = "testPositive";
    testInvestigation.positiveSolution = "";
    testInvestigation.positiveSymptoms.push_back("testSym24");
    testInvestigation.positiveSymptoms.push_back("testSym21");
    testInvestigation.positiveSymptoms.push_back("testSym22");
    testInvestigation.negativeSymptoms.push_back("testSym24");
    testInvestigation.negativeSymptoms.push_back("testSym21");
    testInvestigation.bannedSymptoms.push_back("bannedSym");
    testInvestigation.negativeProblems.push_back("testS23424");
    testInvestigation.negativeProblems.push_back("testS23421");
    testInvestigation.bannedProblems.push_back("bannedPro");
    testInvestigation.negativeSolutions.push_back("123testS23424");
    testInvestigation.bannedSolutions.push_back("bannedSol");
    
    // test JSON serialization
    printf("Testing JSON serialization...\n");
    
    if(!testObject(testCategory, "category"))
        return 1;
    
    if(!testObject(testSymptom, "symptom"))
        return 1;
    
    if(!testObject(testProblem, "problem"))
        return 1;
    
    if(!testObject(testSolution, "solution"))
        return 1;
    
    if(!testObject(testSymptomLink, "symptomLink"))
        return 1;
    
    if(!testObject(testSolutionLink, "solutionLink"))
        return 1;

    if(!testObject(testInvestigation, "investigation"))
        return 1;
    
    // test save / load
    printf("Testing Save/Load...\n");
    
    SystemManager systemManager(new MongoDbDataLayer("localhost:22222", "testDB"));
    DummyRemoteManager dummyManager(systemManager);
    
    if(!testSaveLoadObject(testCategory, "category", dummyManager))
        return 1;
    
    if(!testSaveLoadObject(testSymptom, "symptom", dummyManager))
        return 1;
    
    if(!testSaveLoadObject(testProblem, "problem", dummyManager))
        return 1;
    
    if(!testSaveLoadObject(testSolution, "solution", dummyManager))
        return 1;
    
    if(!testSaveLoadObject(testSymptomLink, "symptomLink", dummyManager))
        return 1;
    
    if(!testSaveLoadObject(testSolutionLink, "solutionLink", dummyManager))
        return 1;
    
    if(!testSaveLoadObject(testInvestigation, "investigation", dummyManager))
        return 1;
    
    
    // test search
    printf("Testing search...\n");
    if(!testSearch("testTag3", testSymptom.id, dummyManager))
        return 1;
    
    /*
    
    std::string serialize(const SystemManager::SearchResult& searchResult);
    std::string serialize(const SolvingMachine::Suggestion& suggestion);
    */
    
    return 0;
}
