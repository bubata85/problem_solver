/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include <mongo/client/dbclient.h>
#include <mongo/client/connpool.h>

/**
 * Wrapper of MongoDB scoped connection because of the difference in the way it is created in versions before 2.0
 */
class MongoConnection
{
public:
    
    MongoConnection(const std::string& connectionString)
    {
#ifdef MONGO_localtime /* defined in 2.0+ only */
        _connection.reset(new mongo::ScopedDbConnection(connectionString));
#else
        _connection.reset(mongo::ScopedDbConnection::getScopedDbConnection(connectionString));
#endif
    }
    
    ~MongoConnection(){}
    
    /*!
     * \brief Returns the associated connection object as reference
     */
    mongo::DBClientBase& conn()
    {
        return _connection->conn();
    }
    
    /*!
     * \brief Call this when you are done with the connection
     */
    void done()
    {
        _connection->done();
    }
    
    /*!
     * \brief Returns a pointer to the associated connection object
     */
    mongo::DBClientBase* operator->() const 
    {
        return _connection->get();
    }
    
private:
    
    boost::scoped_ptr<mongo::ScopedDbConnection> _connection;
    
};
