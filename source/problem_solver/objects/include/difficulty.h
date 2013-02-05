/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#pragma once

namespace ProblemSolver
{

/**
 * Mutual scale for measuring difficulty from 1 to 10.
 * This scale aims to describe how hard is a task based on the work and knowledge required to be done.
 * The actual difficulty depends on the person performing the task.
 * Where rookies would view difficulty 6 as very hard, experts would view it as child's play.
 * That is why when assigning difficulty the system must always provide examples to the user of other tasks with the same difficulty.
 */
enum DifficultyLevel
{
    difficultyUnknown = 0,
    difficultyOneLook = 1,
    difficultyJustByIntuition = 2,
    difficultyJustByLogic = 3,
    difficultySomeExplanation = 4,
    difficultyModerateExplanation = 5,
    difficultyLotsOfExplanation = 6,
    difficultyExplanationMightNotHelp = 7,
    difficultyNeedToHaveDoneIt = 8,
    difficultyManyTimesAndStillNotEasy = 9,
    difficultyCategoryExpertOnly = 10
};

// likelihood of person successfully performing the task
    
} // namespace ProblemSolver
