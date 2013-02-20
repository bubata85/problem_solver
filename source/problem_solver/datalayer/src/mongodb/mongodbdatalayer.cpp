/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "mongodbdatalayer.h"

#include "mongoconnection.h"

#include <boost/format.hpp>
#include <boost/foreach.hpp>

using namespace mongo;

namespace ProblemSolver
{

MongoDbDataLayer::MongoDbDataLayer(const std::string& connectionString, const std::string& database)
{
    _categoryCollection = database + ".categories";
    _symptomCollection = database + ".symptoms";
    _problemCollection = database + ".problems";
    _solutionCollection = database + ".solutions";
    _symptomLinksCollection = database + ".symptomLinks";
    _solutionLinksCollection = database + ".solutionLinks";
    _investigationCollection = database + ".investigations";
}
    
void MongoDbDataLayer::get(const std::vector<Identifier>& categoryIDs, CategoryMap& result, std::vector<Identifier>* notFound)
{
    templateGet(categoryIDs, result, notFound, _categoryCollection);
}
void MongoDbDataLayer::get(const std::vector<Identifier>& problemIDs, ProblemMap& result, std::vector<Identifier>* notFound)
{
    templateGet(problemIDs, result, notFound, _problemCollection);
}
void MongoDbDataLayer::get(const std::vector<Identifier>& symptomIDs, SymptomMap& result, std::vector<Identifier>* notFound)
{
    templateGet(symptomIDs, result, notFound, _symptomCollection);
}
void MongoDbDataLayer::get(const std::vector<Identifier>& solutionIDs, SolutionMap& result, std::vector<Identifier>* notFound)
{
    templateGet(solutionIDs, result, notFound, _solutionCollection);
}
void MongoDbDataLayer::get(const std::vector<Identifier>& symptomLinkIDs, SymptomLinkMap& result, std::vector<Identifier>* notFound)
{
    templateGet(symptomLinkIDs, result, notFound, _symptomLinksCollection);
}
void MongoDbDataLayer::get(const std::vector<Identifier>& solutionLinkIDs, SolutionLinkMap& result, std::vector<Identifier>* notFound)
{
    templateGet(solutionLinkIDs, result, notFound, _solutionLinksCollection);
}
void MongoDbDataLayer::get(const std::vector<Identifier>& investigationIDs, InvestigationMap& result, std::vector<Identifier>* notFound)
{
    templateGet(investigationIDs, result, notFound, _investigationCollection);
}
    
void MongoDbDataLayer::get(const std::vector<Identifier>& problemIDs, ExtendedProblemMap& result, std::vector<Identifier>* notFound)
{
    templateGet(problemIDs, result, notFound, _problemCollection);
}
void MongoDbDataLayer::get(const std::vector<Identifier>& symptomIDs, ExtendedSymptomMap& result, std::vector<Identifier>* notFound)
{
    templateGet(symptomIDs, result, notFound, _symptomCollection);
}
void MongoDbDataLayer::get(const std::vector<Identifier>& solutionIDs, ExtendedSolutionMap& result, std::vector<Identifier>* notFound)
{
    templateGet(solutionIDs, result, notFound, _solutionCollection);
}
    
void MongoDbDataLayer::getLinksByProblem(Identifier problemID, SymptomsWithSameProblem& result, bool* found)
{
    templateGetLinks(problemID, "problemID", "symptomID", result, found, _symptomLinksCollection);
}
void MongoDbDataLayer::getLinksBySymptom(Identifier symptomID, ProblemsWithSameSymptom& result, bool* found)
{
    templateGetLinks(symptomID, "symptomID", "problemID", result, found, _symptomLinksCollection);
}
    
void MongoDbDataLayer::getLinksByProblem(Identifier problemID, SolutionsWithSameProblem& result, bool* found)
{
    templateGetLinks(problemID, "problemID", "solutionID", result, found, _solutionLinksCollection);
}
void MongoDbDataLayer::getLinksBySolution(Identifier solutionID, ProblemsWithSameSolution& result, bool* found)
{
    templateGetLinks(solutionID, "solutionID", "problemID", result, found, _solutionLinksCollection);
}

Identifier MongoDbDataLayer::add(const Category& category)
{
    Identifier newIdentifier = OID::gen().str();
    
    BSONObjBuilder builder;
    makeBson(category, builder, &newIdentifier);
    modifyObject(newIdentifier, builder.done(), _categoryCollection, true);
    
    return newIdentifier;
}
Identifier MongoDbDataLayer::add(const ExtendedProblem& problem)
{
    Identifier newIdentifier = OID::gen().str();
    
    BSONObjBuilder builder;
    makeExtendedInfo(problem, builder, &newIdentifier);
    modifyObject(newIdentifier, builder.done(), _problemCollection, true);
    
    return newIdentifier;
}
Identifier MongoDbDataLayer::add(const ExtendedSymptom& symptom)
{
    Identifier newIdentifier = OID::gen().str();
    
    BSONObjBuilder builder;
    makeExtendedInfo(symptom, builder, &newIdentifier);
    modifyObject(newIdentifier, builder.done(), _symptomCollection, true);
    
    return newIdentifier;
}
Identifier MongoDbDataLayer::add(const ExtendedSolution& solution)
{
    Identifier newIdentifier = OID::gen().str();
    
    BSONObjBuilder builder;
    makeExtendedInfo(solution, builder, &newIdentifier);
    modifyObject(newIdentifier, builder.done(), _solutionCollection, true);
    
    return newIdentifier;
}
Identifier MongoDbDataLayer::add(const SymptomLink& symptomLink)
{
    Identifier newIdentifier = OID::gen().str();
    
    BSONObjBuilder builder;
    makeBson(symptomLink, builder, &newIdentifier);
    modifyObject(newIdentifier, builder.done(), _symptomLinksCollection, true);
    
    return newIdentifier;
}
Identifier MongoDbDataLayer::add(const SolutionLink& solutionLink)
{
    Identifier newIdentifier = OID::gen().str();
    
    BSONObjBuilder builder;
    makeBson(solutionLink, builder, &newIdentifier);
    modifyObject(newIdentifier, builder.done(), _solutionLinksCollection, true);
    
    return newIdentifier;
}
Identifier MongoDbDataLayer::add(const Investigation& investigation)
{
    Identifier newIdentifier = OID::gen().str();
    
    BSONObjBuilder builder;
    makeBson(investigation, builder, &newIdentifier);
    modifyObject(newIdentifier, builder.done(), _investigationCollection, true);
    
    return newIdentifier;
}

void MongoDbDataLayer::modify(const Category& category)
{
    BSONObjBuilder builder;
    makeBson(category, builder);
    modifyObject(category.id, builder.done(), _categoryCollection, false);
}
void MongoDbDataLayer::modify(const ExtendedProblem& problem)
{
    BSONObjBuilder builder;
    makeExtendedInfo(problem, builder);
    modifyObject(problem.id, builder.done(), _problemCollection, false);
}
void MongoDbDataLayer::modify(const ExtendedSymptom& symptom)
{
    BSONObjBuilder builder;
    makeExtendedInfo(symptom, builder);
    modifyObject(symptom.id, builder.done(), _symptomCollection, false);
}
void MongoDbDataLayer::modify(const ExtendedSolution& solution)
{
    BSONObjBuilder builder;
    makeExtendedInfo(solution, builder);
    modifyObject(solution.id, builder.done(), _solutionCollection, false);
}
void MongoDbDataLayer::modify(const SymptomLink& symptomLink)
{
    BSONObjBuilder builder;
    makeBson(symptomLink, builder);
    modifyObject(symptomLink.id, builder.done(), _symptomLinksCollection, false);
}
void MongoDbDataLayer::modify(const SolutionLink& solutionLink)
{
    BSONObjBuilder builder;
    makeBson(solutionLink, builder);
    modifyObject(solutionLink.id, builder.done(), _solutionLinksCollection, false);
}
void MongoDbDataLayer::modify(const Investigation& investigation)
{
    BSONObjBuilder builder;
    makeBson(investigation, builder);
    modifyObject(investigation.id, builder.done(), _investigationCollection, false);
}

void MongoDbDataLayer::remove(const Category& category)
{
    removeObject(BSON("_id" << category.id), _categoryCollection);
}
void MongoDbDataLayer::remove(const Problem& problem)
{
    removeObject(BSON("_id" << problem.id), _problemCollection);
    removeObject(BSON("problemID" << problem.id), _symptomLinksCollection);
    removeObject(BSON("problemID" << problem.id), _solutionLinksCollection);
}
void MongoDbDataLayer::remove(const Symptom& symptom)
{
    removeObject(BSON("_id" << symptom.id), _symptomCollection);
    removeObject(BSON("symptomID" << symptom.id), _symptomLinksCollection);
}
void MongoDbDataLayer::remove(const Solution& solution)
{
    removeObject(BSON("_id" << solution.id), _solutionCollection);
    removeObject(BSON("solutionID" << solution.id), _solutionLinksCollection);
}
void MongoDbDataLayer::remove(const SymptomLink& symptomLink)
{
    removeObject(BSON("_id" << symptomLink.id), _symptomLinksCollection);
}
void MongoDbDataLayer::remove(const SolutionLink& solutionLink)
{
    removeObject(BSON("_id" << solutionLink.id), _solutionLinksCollection);
}
void MongoDbDataLayer::remove(const Investigation& investigation)
{
    removeObject(BSON("_id" << investigation.id), _investigationCollection);
}

/**
 * Used to get symptoms, problems and solutions with one and the same code
 */
template<class T>
void MongoDbDataLayer::templateGet(const std::vector<Identifier>& ids, boost::unordered_map<Identifier, T>& result,
                                   std::vector<Identifier>* notFound, const std::string& collection)
{
    try
    {
        MongoConnection connection(_connectionString);
        
        // some reusable variables
        char index[50];
        index[sizeof(index)-1] = '\0';

        BSONObjBuilder keysArray;

        boost::unordered_set<Identifier> missingIDs;
        auto_ptr<DBClientCursor> dbRecords;
        
        if(!ids.empty())
        {
            // extract the primary keys and any subkeys
            for(unsigned i = 0; i < ids.size(); ++i)
            {
                missingIDs.insert(ids[i]);
                
                snprintf(index, sizeof(index)-1, "%d", i);
                keysArray.append(index, ids[i]);
            }
            
            dbRecords = connection->query(collection, BSON("_id" << BSON("$in" << BSONArray(keysArray.done()))) );
        }
        else
        {
            dbRecords = connection->query(collection, "");
        }

        if(!dbRecords.get()) // it is possible to get here if the connection with the server breaks while executing the query
        {
            printf("Mongo server has gone away!\n");
            throw Exception("Mongo server has gone away!");
        }
        
        while(dbRecords->more())
        {
            BSONObj singleRecord = dbRecords->nextSafe();
            Identifier id = singleRecord["_id"].String();
            
            T& newObject = result[id];
            
            if(!ids.empty())
                missingIDs.erase(id);
            
            readBsonRecord(newObject, singleRecord);
        }

        if(!ids.empty() && !missingIDs.empty())
        {
            if(notFound == NULL)
            {
                throw Exception((boost::format("Missing ID %s in collection %s") % *missingIDs.begin() % collection).str());
            }
            else
            {
                BOOST_FOREACH(CIdentifier id, missingIDs)
                {
                    notFound->push_back(id);
                }
            }
        }
        
        connection.done();
    }
    catch(std::exception& e)
    {
        printf("Error getting records from Mongo collection %s! Error: %s\n", collection.c_str(), e.what());
        throw Exception(e.what());
    }
    catch(...)
    {
        printf("Error getting records from Mongo!\n");
        throw Exception("Error getting records from Mongo");
    }
}

template<class T>
void MongoDbDataLayer::templateGetLinks(CIdentifier byId, const std::string& lookupField, const std::string& organizeField,
                                        boost::unordered_map<Identifier, T>& result, bool* found, const std::string& collection)
{
    try
    {
        MongoConnection connection(_connectionString);

        auto_ptr<DBClientCursor> dbRecords = connection->query(collection, BSON(lookupField << byId) );

        if(!dbRecords.get()) // it is possible to get here if the connection with the server breaks while executing the query
        {
            printf("Mongo server has gone away!\n");
            throw Exception("Mongo server has gone away!");
        }
        
        while(dbRecords->more())
        {
            BSONObj singleRecord = dbRecords->nextSafe();
            Identifier id = singleRecord[organizeField].str();
            
            T& newObject = result[id];
            
            readBsonRecord(newObject, singleRecord);
        }
        
        connection.done();
    }
    catch(std::exception& e)
    {
        printf("Error getting records from Mongo collection %s! Error: %s\n", collection.c_str(), e.what());
        throw Exception(e.what());
    }
    catch(...)
    {
        printf("Error getting records from Mongo!\n");
        throw Exception("Error getting records from Mongo");
    }
    
    if(found != NULL)
        *found = true;
}

template<class T>
void MongoDbDataLayer::readGenericInfo(T& newObject, const BSONObj& singleRecord)
{
    newObject.id = singleRecord["_id"].String();
    newObject.categoryID = singleRecord["categoryID"].String();
    newObject.difficulty = static_cast<DifficultyLevel>(singleRecord["difficulty"].Int());
    newObject.confirmed = singleRecord["confirmed"].Bool();
}

template<class T>
void MongoDbDataLayer::readExtendedInfo(T& newObject, const BSONObj& singleRecord)
{
    readGenericInfo(newObject, singleRecord);
    
    newObject.name = singleRecord["name"].String();
    newObject.description = singleRecord["description"].String();
    
    BSONForEach(tag, singleRecord["tags"].Obj())
    {
        newObject.tags.insert(tag.String());
    }
    
    BSONForEach(step, singleRecord["steps"].Obj())
    {
        newObject.steps.push_back(step.String());
    }
}

void MongoDbDataLayer::readBsonRecord(Category& newObject, const BSONObj& singleRecord)
{
    newObject.id = singleRecord["_id"].String();
    newObject.name = singleRecord["name"].String();
    newObject.description = singleRecord["description"].String();
    newObject.parent = singleRecord["parent"].String();
    
    BSONForEach(child, singleRecord["childs"].Obj())
    {
        newObject.childs.push_back(child.String());
    }
}

void MongoDbDataLayer::readBsonRecord(Problem& newObject, const BSONObj& singleRecord)
{
    readGenericInfo(newObject, singleRecord);
}

void MongoDbDataLayer::readBsonRecord(ExtendedProblem& newObject, const BSONObj& singleRecord)
{
    readExtendedInfo(newObject, singleRecord);
}

void MongoDbDataLayer::readBsonRecord(Symptom& newObject, const BSONObj& singleRecord)
{
    readGenericInfo(newObject, singleRecord);
}

void MongoDbDataLayer::readBsonRecord(ExtendedSymptom& newObject, const BSONObj& singleRecord)
{
    readExtendedInfo(newObject, singleRecord);
}

void MongoDbDataLayer::readBsonRecord(Solution& newObject, const BSONObj& singleRecord)
{
    readGenericInfo(newObject, singleRecord);
}

void MongoDbDataLayer::readBsonRecord(ExtendedSolution& newObject, const BSONObj& singleRecord)
{
    readExtendedInfo(newObject, singleRecord);
}

void MongoDbDataLayer::readBsonRecord(SymptomLink& newObject, const BSONObj& singleRecord)
{
    newObject.id = singleRecord["_id"].String();
    newObject.problemID = singleRecord["problemID"].String();
    newObject.symptomID = singleRecord["symptomID"].String();
    newObject.positiveChecks = singleRecord["positiveChecks"].Int();
    newObject.falsePositiveChecks = singleRecord["falsePositiveChecks"].Int();
    newObject.negativeChecks = singleRecord["negativeChecks"].Int();
    newObject.confirmed = singleRecord["confirmed"].Bool();
}

void MongoDbDataLayer::readBsonRecord(SolutionLink& newObject, const BSONObj& singleRecord)
{
    newObject.id = singleRecord["_id"].String();
    newObject.problemID = singleRecord["problemID"].String();
    newObject.solutionID = singleRecord["solutionID"].String();
    newObject.positive = singleRecord["positive"].Int();
    newObject.negative = singleRecord["negative"].Int();
    newObject.confirmed = singleRecord["confirmed"].Bool();
}

void MongoDbDataLayer::readBsonRecord(Investigation& newObject, const BSONObj& singleRecord)
{
    newObject.id = singleRecord["_id"].String();
    newObject.closed = singleRecord["closed"].Bool();
    newObject.positiveProblem = singleRecord["positiveProblem"].String();
    newObject.positiveSolution = singleRecord["positiveSolution"].String();
    
    BSONForEach(identifier, singleRecord["positiveSymptoms"].Obj())
    {
        newObject.positiveSymptoms.push_back(identifier.String());
    }
    BSONForEach(identifier, singleRecord["negativeSymptoms"].Obj())
    {
        newObject.negativeSymptoms.push_back(identifier.String());
    }
    BSONForEach(identifier, singleRecord["bannedSymptoms"].Obj())
    {
        newObject.bannedSymptoms.push_back(identifier.String());
    }
    
    BSONForEach(identifier, singleRecord["negativeProblems"].Obj())
    {
        newObject.negativeProblems.push_back(identifier.String());
    }
    BSONForEach(identifier, singleRecord["bannedProblems"].Obj())
    {
        newObject.bannedProblems.push_back(identifier.String());
    }
    
    BSONForEach(identifier, singleRecord["negativeSolutions"].Obj())
    {
        newObject.negativeSolutions.push_back(identifier.String());
    }
    BSONForEach(identifier, singleRecord["bannedSolutions"].Obj())
    {
        newObject.bannedSolutions.push_back(identifier.String());
    }
}

void MongoDbDataLayer::modifyObject(CIdentifier id, const mongo::BSONObj& object, const std::string& collection, bool insert)
{
    try
    {
        MongoConnection connection(_connectionString);

        connection->update(collection, BSON("_id" << id), object, insert);

        connection.done();
    }
    catch(std::exception& e)
    {
        printf("Error updating records in Mongo collection %s! Error: %s\n", collection.c_str(), e.what());
        throw Exception(e.what());
    }
    catch(...)
    {
        printf("Error updating records in Mongo!\n");
        throw Exception("Error updating records in Mongo");
    }
}

void MongoDbDataLayer::removeObject(const mongo::BSONObj& query, const std::string& collection)
{
    try
    {
        MongoConnection connection(_connectionString);

        connection->remove(collection, query);

        connection.done();
    }
    catch(std::exception& e)
    {
        printf("Error removing records from Mongo collection %s! Error: %s\n", collection.c_str(), e.what());
        throw Exception(e.what());
    }
    catch(...)
    {
        printf("Error removing records from Mongo!\n");
        throw Exception("Error removing records from Mongo");
    }
}

template<class T>
void MongoDbDataLayer::makeExtendedInfo(const T& newObject, mongo::BSONObjBuilder& singleRecord, const Identifier* customIdentifier)
{
    if(customIdentifier != NULL)
        singleRecord.append("_id", *customIdentifier);
    else
        singleRecord.append("_id", newObject.id);
    
    singleRecord.append("categoryID", newObject.categoryID);
    singleRecord.append("difficulty", static_cast<int>(newObject.difficulty));
    singleRecord.append("confirmed", newObject.confirmed);
    
    singleRecord.append("name", newObject.name);
    singleRecord.append("description", newObject.description);
    
    std::vector<std::string> vectorTags;
    BOOST_FOREACH(const std::string& tag, newObject.tags)
    {
        vectorTags.push_back(tag);
    }
    
    singleRecord.append("tags", vectorTags);
    singleRecord.append("steps", newObject.steps);
}

void MongoDbDataLayer::makeBson(const Category& newObject, mongo::BSONObjBuilder& singleRecord, const Identifier* customIdentifier)
{
    if(customIdentifier != NULL)
        singleRecord.append("_id", *customIdentifier);
    else
        singleRecord.append("_id", newObject.id);
    
    singleRecord.append("name", newObject.name);
    singleRecord.append("description", newObject.description);
    singleRecord.append("parent", newObject.parent);
    singleRecord.append("childs", newObject.childs);
}

void MongoDbDataLayer::makeBson(const SymptomLink& newObject, mongo::BSONObjBuilder& singleRecord, const Identifier* customIdentifier)
{
    if(customIdentifier != NULL)
        singleRecord.append("_id", *customIdentifier);
    else
        singleRecord.append("_id", newObject.id);
    
    singleRecord.append("problemID", newObject.problemID);
    singleRecord.append("symptomID", newObject.symptomID);
    
    singleRecord.append("positiveChecks", newObject.positiveChecks);
    singleRecord.append("falsePositiveChecks", newObject.falsePositiveChecks);
    singleRecord.append("negativeChecks", newObject.negativeChecks);
    
    singleRecord.append("confirmed", newObject.confirmed);
}

void MongoDbDataLayer::makeBson(const SolutionLink& newObject, mongo::BSONObjBuilder& singleRecord, const Identifier* customIdentifier)
{
    if(customIdentifier != NULL)
        singleRecord.append("_id", *customIdentifier);
    else
        singleRecord.append("_id", newObject.id);
    
    singleRecord.append("problemID", newObject.problemID);
    singleRecord.append("solutionID", newObject.solutionID);
    
    singleRecord.append("positive", newObject.positive);
    singleRecord.append("negative", newObject.negative);
    
    singleRecord.append("confirmed", newObject.confirmed);
}

void MongoDbDataLayer::makeBson(const Investigation& newObject, mongo::BSONObjBuilder& singleRecord, const Identifier* customIdentifier)
{
    if(customIdentifier != NULL)
        singleRecord.append("_id", *customIdentifier);
    else
        singleRecord.append("_id", newObject.id);
    
    singleRecord.append("closed", newObject.closed);
    
    singleRecord.append("positiveProblem", newObject.positiveProblem);
    singleRecord.append("positiveSolution", newObject.positiveSolution);
    
    singleRecord.append("positiveSymptoms", newObject.positiveSymptoms);
    singleRecord.append("negativeSymptoms", newObject.negativeSymptoms);
    singleRecord.append("bannedSymptoms", newObject.bannedSymptoms);
    
    singleRecord.append("negativeProblems", newObject.negativeProblems);
    singleRecord.append("bannedProblems", newObject.bannedProblems);
    
    singleRecord.append("negativeSolutions", newObject.negativeSolutions);
    singleRecord.append("bannedSolutions", newObject.bannedSolutions);
}

} // namespace ProblemSolver
