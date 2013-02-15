/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "remotejsonmanager.h"
#include "systemmanager.h"
#include "jsonserialization.h"

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace boost::property_tree;

namespace ProblemSolver
{

bool RemoteJsonManager::_stopAllManagers = false;
    
RemoteJsonManager::RemoteJsonManager(SystemManager& systemManager):
    _systemManager(systemManager)
{
}

void RemoteJsonManager::run(const std::string& host, int port)
{
    printf("RemoteJsonManager: Starting instance on %s:%d\n", host.c_str(), port);
    
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0)
    {
        printMessage("RemoteJsonManager: ERROR opening socket");
        return;
    }

    int flags = fcntl(serverSocket, F_GETFL, 0);
    if(flags < 0)
    {
        printMessage("RemoteJsonManager: ERROR getting socket options");
        return;
    }
    flags = (flags|O_NONBLOCK);
    if(fcntl(serverSocket, F_SETFL, flags) != 0)
    {
        printMessage("RemoteJsonManager: ERROR setting socket options");
        return;
    }
    
    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(host.c_str());
    serverAddress.sin_port = htons(port);

    if(bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printMessage("RemoteJsonManager: ERROR binding socket");
        return;
    }
    
    listen(serverSocket,5);
    
    while(!_stopAllManagers)
    {
        int newSocket = accept(serverSocket, NULL, NULL);
        if(newSocket < 0)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
                usleep(10000); // sleep 10ms
            else
            {
                printMessage("RemoteJsonManager: ERROR during accept");
                break; // exit the server
            }
        }
        else
        {
            /** \todo Lubo: server should be multithreaded some day */
            onNewConnection(newSocket);
        }
    }
    
    printf("RemoteJsonManager: Stopping instance on %s:%d\n", host.c_str(), port);
    close(serverSocket);
}

/**
 * Signal all managers to stop
 */
void RemoteJsonManager::stopAll()
{
    _stopAllManagers = true;
}

/**
 * Reads the request and calls processRequest on it then closes the socket
 */
void RemoteJsonManager::onNewConnection(int clientSocket)
{
    std::string fullRequest;
    std::string response;
    
    char buffer[256];
    int bytesRead = 0;
    
    do
    {
        bytesRead = read(clientSocket, buffer, 255);
        if(bytesRead < 0)
        {
            static const std::string error = "RemoteJsonManager: ERROR reading request.";
            printMessage(error.c_str());
            sendResponseAndClose(clientSocket, error, true);
            return;
        }
        
        fullRequest.append(buffer, bytesRead);
        
        if(fullRequest.size() > 10000)
        {
            static const std::string error = "RemoteJsonManager: ERROR too long request.";
            printMessage(error.c_str());
            sendResponseAndClose(clientSocket, error, true);
            return;
        }
        
    }while(bytesRead == 255);

    bool error = true;
    try
    {
        response = processRequest(fullRequest);
        error = false;
    }
    catch(BaseException& exception)
    {
        response = exception.what();
        printMessage(response.c_str());
    }
    catch(...)
    {
        response = "Unknown error";
        printMessage(response.c_str());
    }
    
    sendResponseAndClose(clientSocket, response, error);
}

/**
 * Processes the supplied request
 */
std::string RemoteJsonManager::processRequest(const std::string& request)
{
    printf("Processing request: %s\n", request.c_str());
    std::string result;
    
    std::vector<std::string> lines;
    boost::algorithm::split(lines, request, boost::algorithm::is_any_of("\n"));

    // parse headers
    uint i = 1; // always skip the first line
    for(; i < lines.size(); ++i)
    {
        if(lines[i].find(':') == std::string::npos)
            break;
    }
    
    std::string requestBody;
    
    ++i; // always skip one line after the headers
    for(; i < lines.size(); ++i)
    {
        requestBody.append(lines[i]);
    }
    
    printf("Request Body: %s\n", requestBody.c_str());
    
    std::stringstream jsonStream;
    jsonStream << requestBody;
    ptree jsonTree;
    try
    {
        /** \todo Lubo: implement all requests and responses! */
        
        json_parser::read_json(jsonStream, jsonTree);
        
        std::string type = jsonTree.get<std::string>("RequestType");
        if(type == "database")
        {
            // these requests are linked with get/add/modify/delete
            //std::string operation = jsonTree.get<std::string>("operation");
            
            // process the request
            
            ExtendedSymptom symptom;
            symptom.id = 1111;
            symptom.confirmed = false;
            symptom.difficulty = difficultyCategoryExpertOnly;
            symptom.categoryID = 2222;
            symptom.description = "test description";
            symptom.name = "test name";
            symptom.steps.push_back("step1");
            symptom.steps.push_back("step2");
            symptom.steps.push_back("step3");
            symptom.tags.insert("tag1");
            symptom.tags.insert("tag2");
            
            JsonSerializer serializer;
            result = serializer.serialize(symptom);
        }
        else
        {
            std::string message = "Unknown RequestType " + type;
            throw std::runtime_error(message);
        }
    }
    catch (std::runtime_error& err)
    {
        std::string message = "RemoteJsonManager: ERROR Invalid JSON - ";
        message += err.what();
        throw Exception(message);
    }
    
    return result;
}

/**
 * Sends a response to the client and closes the socket
 */
void RemoteJsonManager::sendResponseAndClose(int clientSocket, const std::string& response, bool error)
{
    int written = 0;
    if(error)
    {
        std::string actualMessage;
        actualMessage.reserve(response.size() + 10);
        
        actualMessage.append("{\"error\": \"");
        actualMessage.append(response);
        actualMessage.append("\"}");
        
        written = write(clientSocket, actualMessage.c_str(), actualMessage.size());
    }
    else
    {
        written = write(clientSocket, response.c_str(), response.size());
    }
    
    
    if (written < 0)
        printf("RemoteJsonManager: ERROR sending response %s\n", response.c_str());
    
    close(clientSocket);
}

/**
 * Prints messages
 */
void RemoteJsonManager::printMessage(const char* message)
{
    printf("%s\n", message);
}

std::string RemoteJsonManager::performGetOrDelete(bool isGet, const boost::property_tree::ptree& json)
{
    std::string response;
    
    std::string objectType = json.get<std::string>("objectType");
    /** \todo Lubo: implement these! */
    
    return response;
}

std::string RemoteJsonManager::performAddOrModify(bool isAdd, const boost::property_tree::ptree& json)
{
    std::string response;
    
    std::string objectType = json.get<std::string>("objectType");
    /** \todo Lubo: implement these! */
    
    return response;
}

} // namespace ProblemSolver
