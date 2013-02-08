/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include <vector>

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
    
    void run(const char* host, int port);
    
public:
    
    static void stopAll();
    
private:
    
    void onNewConnection(int clientSocket);
    std::string processRequest(const std::string& request);
    void sendResponseAndClose(int clientSocket, const std::string& response);
    
private:
    
    SystemManager& _systemManager;

private:
    
    static bool _stopAllManagers;
    
};

} // namespace ProblemSolver
