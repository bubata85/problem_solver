/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "baseexception.h"
#include "identifier.h"

#include <vector>
#include <boost/property_tree/ptree.hpp>

namespace ProblemSolver
{

class SystemManager;
    
/**
 * This class groups all functionality connected with making suggestions about how to identify unknown problems
 */
class RemoteJsonManager
{
public:
    
    RemoteJsonManager(SystemManager& systemManager);
    ~RemoteJsonManager(){}
    
public:
    
    /**
     * Exception thrown from RemoteJsonManager operations
     */
    class Exception: public BaseException
    {
    public:
        explicit Exception(const std::string& errorMessage):
            BaseException(errorMessage){}
            
        virtual ExceptionCode getCode() const { return exceptionCodeSystemManager; }
    };
    
public:
    
    void run(const std::string& host, int port);
    
public:
    
    static void stopAll();
    
protected:
    
    void onNewConnection(int clientSocket);
    std::string processRequest(const std::string& request);
    void sendResponseAndClose(int clientSocket, const std::string& response, bool error);
    
private:
    
    void printMessage(const char* message);
    
private:
    
    template<class T>
    std::string performDatabaseOperation(const boost::property_tree::ptree& json);
    
    template<class T>
    std::string performGet(const std::vector<Identifier>& identifiers);
    
    template<class T>
    std::string performDelete(const std::vector<Identifier>& identifiers);
    
    template<class T>
    std::string performAddOrModify(bool isAdd, const boost::property_tree::ptree& json);
    
private:
    
    SystemManager& _systemManager;

private:
    
    static bool _stopAllManagers;
    
};

} // namespace ProblemSolver
