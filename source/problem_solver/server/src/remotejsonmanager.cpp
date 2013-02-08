/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

#include "remotejsonmanager.h"
#include "systemmanager.h"

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

namespace ProblemSolver
{

RemoteJsonManager::RemoteJsonManager(SystemManager& systemManager):
    _systemManager(systemManager)
{
}

void RemoteJsonManager::run(const char* host, int port)
{
    printf("RemoteJsonManager: Starting instance on %s:%d", host, port);
    
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0) 
        printf("RemoteJsonManager: ERROR opening socket\n");

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(host);
    serverAddress.sin_port = htons(port);

    if(bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
        printf("RemoteJsonManager: ERROR binding socket");
    
    listen(serverSocket,5);
    
    while(_stopAllManagers)
    {
        int newSocket = accept(serverSocket, NULL, NULL);
        if(newSocket < 0)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
                usleep(10000); // sleep 10ms
            else
            {
                printf("RemoteJsonManager: ERROR during accept");
                break; // exit the server
            }
        }
        else
        {
            onNewConnection(newSocket);
        }
    }
    
    printf("RemoteJsonManager: Stopping instance on %s:%d", host, port);
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
    
    do
    {
        int bytesRead = read(clientSocket, buffer, 255);
        if(bytesRead < 0)
        {
            printf("RemoteJsonManager: ERROR reading request.");
            sendResponseAndClose(clientSocket, "ERROR reading request");
            return;
        }
        
        fullRequest.append(buffer, bytesRead);
        
        if(fullRequest.size() > 10000)
        {
            printf("RemoteJsonManager: ERROR too long request.");
            sendResponseAndClose(clientSocket, "ERROR too long request");
            return;
        }
        
    }while(bytesRead == 255);
    
    
    //parse the json
    std::stringstream jsonStream;
    jsonStream << fullRequest;
    ptree jsonTree;
    try
    {
        json_parser::read_json(jsonStream, jsonTree);
    }
    catch (json_parser_error& err)
    {
        printf("RemoteJsonManager: ERROR Invalid JSON %s", fullRequest);
        sendResponseAndClose(clientSocket, "ERROR Invalid JSON");
        return;
    }
    
    try
    {
        json_parser::read_json(jsonStream, jsonTree);
    }
    catch (json_parser_error& err)
    {
        printf("RemoteJsonManager: ERROR Invalid JSON %s", fullRequest);
        sendResponseAndClose(clientSocket, "ERROR Invalid JSON");
        return;
    }
    
/*
    //populate the request
    if (tree.get<std::string>("Type", "FastSearch") == "FastSearch")
    {
        std::auto_ptr<FastSearchRequest> request(new FastSearchRequest);

        parseFastSearch(tree, *request);

        return request.release();
    }
    else
        throw ServerValidationException("Unknown request type: " + tree.get<std::string>("Type"));
    
    
    
    
    
    
    std::string response = processRequest()
    
    printf("Here is the message: %s\n",buffer);
    int n = write(newsockfd,"I got your message",18);
    if (n < 0) error("ERROR writing to socket");
    close(newsockfd);
    close(sockfd);*/
}

/**
 * Processes the supplied request
 */
std::string RemoteJsonManager::processRequest(const std::string& request)
{
    
}

/**
 * Sends a response to the client and closes the socket
 */
void RemoteJsonManager::sendResponseAndClose(int clientSocket, const std::string& response)
{
    int written = write(clientSocket, response.c_str(), response.size());
    if (written < 0)
        printf("RemoteJsonManager: ERROR sending response %s", response);
    
    close(clientSocket);
}

} // namespace ProblemSolver
