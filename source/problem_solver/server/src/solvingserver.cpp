/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "cachingdatalayer.h"
#include "memorydatalayer.h"
#include "mysqldatalayer.h"
#include "mongodbdatalayer.h"

#include "systemmanager.h"
#include "remotejsonmanager.h"

#include <stdio.h>
#include <signal.h>
#include <boost/program_options.hpp>

using namespace ProblemSolver;
namespace po = boost::program_options;

/**
 * Handles CTRL+C signal
 */
void interruptHandler(int signal)
{
    printf("Caught signal %d\n", signal);
    RemoteJsonManager::stopAll();
}

/**
 * Initializes signal handlers
 */
void initHandlers()
{
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = interruptHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
}

#include "boost/property_tree/json_parser.hpp"
#include "boost/property_tree/info_parser.hpp"

int main(int argc, const char* argv[])
{
    initHandlers();
    
    std::string host;
    int port;
    std::string mongoConnectionString;
    std::string mongoDatabase;
    
    po::variables_map optionsMap;
    try
    {
        po::options_description allowedOptions(200, 100);
        allowedOptions.add_options()
            ("help,h", "Produce help message.")
            ("host", po::value<std::string>()->required(), "Required. Server IP")
            ("port", po::value<int>()->required(), "Required. Server Port")
            ("mongoConnection", po::value<std::string>()->required(), "Required. Mongo Connection string. E.g: host:port")
            ("mongoDatabase", po::value<std::string>()->required(), "Required. Mongo Database name. E.g: kb");

        po::store(po::parse_command_line(argc, argv, allowedOptions), optionsMap, true);
        
        if (optionsMap.count("help") || optionsMap.empty())
        {
            std::cout << allowedOptions << "\n";
            return 1;
        }

        po::notify(optionsMap);
        
        host = optionsMap["host"].as<std::string>();
        port = optionsMap["port"].as<int>();
        mongoConnectionString = optionsMap["mongoConnection"].as<std::string>();
        mongoDatabase = optionsMap["mongoDatabase"].as<std::string>();
    }
    catch(std::exception& e)
    {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }
    
    /** \todo Lubo: in the future the caching must be implemented */
    // SystemManager systemManager(new CachingDataLayer(new MongoDbDataLayer(), new MemoryDataLayer()));
    SystemManager systemManager(new MongoDbDataLayer(mongoConnectionString, mongoDatabase));
    
    RemoteJsonManager remoteJsonManager(systemManager);
    remoteJsonManager.run(host, port);
    
    return 0;
}
