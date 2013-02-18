/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "datalayer.h"

namespace mongo
{
class BSONObj;
class BSONObjBuilder;
}

namespace ProblemSolver
{

/**
 * Datalayer using MongoDB for storage
 */
class MongoDbDataLayer: public IDataLayer
{
public:
    
    MongoDbDataLayer(const std::string& connectionString, const std::string& database);
    virtual ~MongoDbDataLayer(){}

public:
    
    /**
     * Exception thrown from all operations of MongoDB
     */
    class Exception: public DataLayerException
    {
    public:
        explicit Exception(const std::string& errorMessage):
            DataLayerException(errorMessage){}
    };

public:
    
    /** \todo Lubo: implement the mongoDB storage! */
    virtual void get(const std::vector<Identifier>& categoryIDs, CategoryMap& result, std::vector<Identifier>* notFound = NULL);
    virtual void get(const std::vector<Identifier>& problemIDs, ProblemMap& result, std::vector<Identifier>* notFound = NULL);
    virtual void get(const std::vector<Identifier>& symptomIDs, SymptomMap& result, std::vector<Identifier>* notFound = NULL);
    virtual void get(const std::vector<Identifier>& solutionIDs, SolutionMap& result, std::vector<Identifier>* notFound = NULL);
    virtual void get(const std::vector<Identifier>& investigationIDs, InvestigationMap& result, std::vector<Identifier>* notFound = NULL);

    virtual void get(const std::vector<Identifier>& problemIDs, ExtendedProblemMap& result, std::vector<Identifier>* notFound = NULL);
    virtual void get(const std::vector<Identifier>& symptomIDs, ExtendedSymptomMap& result, std::vector<Identifier>* notFound = NULL);
    virtual void get(const std::vector<Identifier>& solutionIDs, ExtendedSolutionMap& result, std::vector<Identifier>* notFound = NULL);

    virtual void getLinksByProblem(Identifier problemID, SymptomsWithSameProblem& result, bool* found = NULL);
    virtual void getLinksBySymptom(Identifier symptomID, ProblemsWithSameSymptom& result, bool* found = NULL);
    
    virtual void getLinksByProblem(Identifier problemID, SolutionsWithSameProblem& result, bool* found = NULL);
    virtual void getLinksBySolution(Identifier solutionID, ProblemsWithSameSolution& result, bool* found = NULL);
    
public:

    virtual Identifier add(const Category& category);
    virtual Identifier add(const ExtendedProblem& problem);
    virtual Identifier add(const ExtendedSymptom& symptom);
    virtual Identifier add(const ExtendedSolution& solution);
    virtual Identifier add(const SymptomLink& symptomLink);
    virtual Identifier add(const SolutionLink& solutionLink);
    virtual Identifier add(const Investigation& investigation);

    virtual void modify(const Category& category);
    virtual void modify(const ExtendedProblem& problem);
    virtual void modify(const ExtendedSymptom& symptom);
    virtual void modify(const ExtendedSolution& solution);
    virtual void modify(const SymptomLink& symptomLink);
    virtual void modify(const SolutionLink& solutionLink);
    virtual void modify(const Investigation& investigation);

    virtual void remove(const Category& category);
    virtual void remove(const Problem& problem);
    virtual void remove(const Symptom& symptom);
    virtual void remove(const Solution& solution);
    virtual void remove(const SymptomLink& symptomLink);
    virtual void remove(const SolutionLink& solutionLink);
    virtual void remove(const Investigation& investigation);
    
private:
    
    // functions used in GET operations
    
    template<class T>
    void templateGet(const std::vector<Identifier>& ids, boost::unordered_map<Identifier, T>& result,
                     std::vector<Identifier>* notFound, const std::string& collection);
    
    template<class T>
    void templateGetLinks(CIdentifier byId, const std::string& lookupField, const std::string& organizeField,
                          boost::unordered_map<Identifier, T>& result, bool* found, const std::string& collection);
    
    template<class T>
    void readGenericInfo(T& newObject, const mongo::BSONObj& singleRecord);
    
    template<class T>
    void readExtendedInfo(T& newObject, const mongo::BSONObj& singleRecord);
    
    void readBsonRecord(Category& newObject, const mongo::BSONObj& singleRecord);
    void readBsonRecord(Problem& newObject, const mongo::BSONObj& singleRecord);
    void readBsonRecord(ExtendedProblem& newObject, const mongo::BSONObj& singleRecord);
    void readBsonRecord(Symptom& newObject, const mongo::BSONObj& singleRecord);
    void readBsonRecord(ExtendedSymptom& newObject, const mongo::BSONObj& singleRecord);
    void readBsonRecord(Solution& newObject, const mongo::BSONObj& singleRecord);
    void readBsonRecord(ExtendedSolution& newObject, const mongo::BSONObj& singleRecord);
    void readBsonRecord(SymptomLink& newObject, const mongo::BSONObj& singleRecord);
    void readBsonRecord(SolutionLink& newObject, const mongo::BSONObj& singleRecord);
    void readBsonRecord(Investigation& newObject, const mongo::BSONObj& singleRecord);
    
private:
    
    void modifyObject(CIdentifier id, const mongo::BSONObj& object, const std::string& collection, bool insert);
    void removeObject(const mongo::BSONObj& query, const std::string& collection);

    template<class T>
    void makeExtendedInfo(const T& newObject, mongo::BSONObjBuilder& singleRecord, const Identifier* customIdentifier = NULL);
    
    void makeBson(const Category& newObject, mongo::BSONObjBuilder& singleRecord, const Identifier* customIdentifier = NULL);
    void makeBson(const SymptomLink& newObject, mongo::BSONObjBuilder& singleRecord, const Identifier* customIdentifier = NULL);
    void makeBson(const SolutionLink& newObject, mongo::BSONObjBuilder& singleRecord, const Identifier* customIdentifier = NULL);
    void makeBson(const Investigation& newObject, mongo::BSONObjBuilder& singleRecord, const Identifier* customIdentifier = NULL);
    
private:
    
    std::string _connectionString;
    
    std::string _categoryCollection;
    std::string _symptomCollection;
    std::string _problemCollection;
    std::string _solutionCollection;
    std::string _symptomLinksCollection;
    std::string _solutionLinksCollection;
    std::string _investigationCollection;
    
};

} // namespace ProblemSolver
