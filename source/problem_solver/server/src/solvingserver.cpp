/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include <stdio.h>

#include "systemmanager.h"

#include "cachingdatalayer.h"
#include "memorydatalayer.h"
#include "mysqldatalayer.h"

using namespace ProblemSolver;

int main(int argc, const char* argv[])
{
    SystemManager manager(new CachingDataLayer(new MySqlDataLayer(), new MemoryDataLayer()));
    
    
    
    printf("Hello world!\n");
    return 0;
}
