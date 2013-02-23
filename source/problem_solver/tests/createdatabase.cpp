/**
 * ProblemSolver - Self-service problem identification and fixing solution
 *
 *  Copyright (C) 2013 Lyubomir Stankov.
 *  This program is free software: you can redistribute it and/or modify it under the terms of the FreeBSD license.
 */

#include "mongodbdatalayer.h"
#include "systemmanager.h"

#include <stdio.h>

using namespace ProblemSolver;

void addSymptomLink(const ExtendedSymptom& symptom, const ExtendedProblem& problem, int positive, int falsePositive, int negative, IDataLayer& layer)
{
    SymptomLink newSymptomLink;
    newSymptomLink.id = "";
    newSymptomLink.problemID = problem.id;
    newSymptomLink.symptomID = symptom.id;
    newSymptomLink.positiveChecks = positive;
    newSymptomLink.falsePositiveChecks = falsePositive;
    newSymptomLink.negativeChecks = negative;
    newSymptomLink.confirmed = true;
    
    layer.add(newSymptomLink);
}

void addSolutionLink(const ExtendedSolution& solution, const ExtendedProblem& problem, int positive, int negative, IDataLayer& layer)
{
    SolutionLink newLink;
    newLink.id = "";
    newLink.problemID = problem.id;
    newLink.solutionID = solution.id;
    newLink.positive = positive;
    newLink.negative = negative;
    newLink.confirmed = true;
    
    layer.add(newLink);
}


int main(int argc, const char* argv[])
{
    MongoDbDataLayer dataLayer("localhost:22222", "isp_kb");
    
    /** LAYOUT
     
        // categories
        1. general
        2. lan
        3. wireless


        // symptoms
        1. can't ping localhost
        2. can access only some sites
        3. network stays disconnected
        4. can't ping router/switch
        5. message pops in browser when site doesnt load
        6. no problem can be detected
        7. the lan light between the router/computer back is not on
        8. ping only to specific sites is missing
        9. messages from firewall pop from time to time
        10. can't find wireless network
        11. network icon missing


        // problems
        1. lan card hardware problem - 1, 7, 11, 10
        2. problem in ISP - 2, 6, 8
        3. stuck/off router/switch - 3, 4, 7, 10
        4. firewall blocking network - 2, 4, 9
        5. site problem - 2, 8
        6. router/switch hardware problem - 3, 4, 7, 10
        7. incorrect lan / router configuration - 3, 4
        8. stuck lan card - 3, 4, 7, 10
        9. browser settings - 2, 5
        10. wireless not turned on - 3, 4, 11
        11. device not installed - 11, 1


        // solutions
        1. replace problematic lan card - 1, 8
        2. contact ISP - 2
        3. restart router/switch - 3
        4. replace problematic router/switch - 3, 6
        5. contact site owner if needed - 4
        6. enter correct lan / router settings - 7
        7. restart computer - 8
        8. change browser settings - 9
        9. turn on wireless - 10
        10. install drivers for the device - 11
    */
    
    // Create categories
    // categories
    //    1. general
    //    2. lan
    //    3. wireless
    
    
    Category categoryGeneral;
    categoryGeneral.id = "";
    categoryGeneral.name = "General";
    categoryGeneral.description = "General category for everything shared between child categories";
    categoryGeneral.parent = "";

    categoryGeneral.id = dataLayer.add(categoryGeneral);
    
    Category categoryLan;
    categoryLan.id = "";
    categoryLan.name = "Lan";
    categoryLan.description = "Cable Lan related symptoms/problems/solutions";
    categoryLan.parent = categoryGeneral.id;
    
    categoryLan.id = dataLayer.add(categoryLan);
    
    Category categoryWireless;
    categoryWireless.id = "";
    categoryWireless.name = "Wireless";
    categoryWireless.description = "Wireless related symptoms/problems/solutions";
    categoryWireless.parent = categoryGeneral.id;
    
    categoryWireless.id = dataLayer.add(categoryWireless);
    
    categoryGeneral.childs.push_back(categoryLan.id);
    categoryGeneral.childs.push_back(categoryWireless.id);
    
    dataLayer.modify(categoryGeneral);
    
    
    // Create Symptoms
    // symptoms
    //    1. can't ping localhost
    //    2. can access only some sites
    //    3. network stays disconnected
    //    4. can't ping router/switch
    //    5. message pops in browser when site doesnt load
    //    6. no problem can be detected
    //    7. the lan light between the router/computer back is not on
    //    8. ping only to specific sites is missing
    //    9. messages from firewall pop from time to time
    //    10. can't find wireless network
    //    11. network icon missing
    
    ExtendedSymptom symptom1;
    symptom1.id = "";
    symptom1.categoryID = categoryGeneral.id;
    symptom1.difficulty = difficultySomeExplanation;
    symptom1.confirmed = true;
    symptom1.name = "No localhost ping";
    symptom1.description = "Ping command on localhost fails or timeouts";
    symptom1.tags.insert("localhost");
    symptom1.tags.insert("ping");
    symptom1.steps.push_back("Open command prompt");
    symptom1.steps.push_back("Type 'ping localhost'");
    symptom1.steps.push_back("Check if the ping fails");
    
    symptom1.id = dataLayer.add(symptom1);
    
    
    ExtendedSymptom symptom2;
    symptom2.id = "";
    symptom2.categoryID = categoryGeneral.id;
    symptom2.difficulty = difficultyJustByIntuition;
    symptom2.confirmed = true;
    symptom2.name = "No access to specific sites";
    symptom2.description = "Access generally works but there is no access to some of the sites.";
    symptom2.tags.insert("access");
    symptom2.tags.insert("specific");
    symptom2.tags.insert("sites");
    symptom2.steps.push_back("Open several different international sites.");
    symptom2.steps.push_back("Open several different local sites.");
    symptom2.steps.push_back("Check if any of them is not working.");
    
    symptom2.id = dataLayer.add(symptom2);
    
    
    ExtendedSymptom symptom3;
    symptom3.id = "";
    symptom3.categoryID = categoryGeneral.id;
    symptom3.difficulty = difficultyJustByIntuition;
    symptom3.confirmed = true;
    symptom3.name = "Network is disconnected";
    symptom3.description = "Network icon shows a disconnected status";
    symptom3.tags.insert("network");
    symptom3.tags.insert("disconnected");
    symptom3.steps.push_back("If there is icon on the tray check if it is crossed.");
    symptom3.steps.push_back("If there is no icon on tray open network connections and check if the network has a cross.");
    
    symptom3.id = dataLayer.add(symptom3);
    
    
    ExtendedSymptom symptom4;
    symptom4.id = "";
    symptom4.categoryID = categoryGeneral.id;
    symptom4.difficulty = difficultyModerateExplanation;
    symptom4.confirmed = true;
    symptom4.name = "No router / switch ping";
    symptom4.description = "Ping command to the router or switch fails or timeouts";
    symptom4.tags.insert("router");
    symptom4.tags.insert("switch");
    symptom4.tags.insert("ping");
    symptom4.steps.push_back("Double-click network tray icon and switch to tab details.");
    symptom4.steps.push_back("Write down the address of 'Gateway'");
    symptom4.steps.push_back("Open command prompt");
    symptom4.steps.push_back("Type 'ping ' followed by the address you wrote down.");
    symptom4.steps.push_back("Check if the ping fails");
    
    symptom4.id = dataLayer.add(symptom4);
    
    
    ExtendedSymptom symptom5;
    symptom5.id = "";
    symptom5.categoryID = categoryGeneral.id;
    symptom5.difficulty = difficultyJustByLogic;
    symptom5.confirmed = true;
    symptom5.name = "Warning message in browser";
    symptom5.description = "A warning message pops up in browser when opening sites";
    symptom5.tags.insert("warning");
    symptom5.tags.insert("message");
    symptom5.tags.insert("browser");
    symptom5.steps.push_back("Open a site that you know can't load.");
    symptom5.steps.push_back("Look for messages from the browser.");
    symptom5.steps.push_back("Check if the message contains 'blocked'.");
    
    symptom5.id = dataLayer.add(symptom5);
    
    
    ExtendedSymptom symptom6;
    symptom6.id = "";
    symptom6.categoryID = categoryGeneral.id;
    symptom6.difficulty = difficultyOneLook;
    symptom6.confirmed = true;
    symptom6.name = "No problem can be detected";
    symptom6.description = "Cannot locate any problem";
    symptom6.tags.insert("detect");
    symptom6.tags.insert("problem");
    symptom6.steps.push_back("Check if you have checked all symptoms and problems.");
    
    symptom6.id = dataLayer.add(symptom6);
    
    
    ExtendedSymptom symptom7;
    symptom7.id = "";
    symptom7.categoryID = categoryLan.id;
    symptom7.difficulty = difficultyJustByLogic;
    symptom7.confirmed = true;
    symptom7.name = "Lan card/Router connection light is not on";
    symptom7.description = "Cannot locate any problem";
    symptom7.tags.insert("connection");
    symptom7.tags.insert("light");
    symptom7.tags.insert("lan");
    symptom7.tags.insert("router");
    symptom7.steps.push_back("Check if the light on the back of the computer where the cable goes in is on.");
    symptom7.steps.push_back("Check if the light on the router where the cable goes in is on.");
    
    symptom7.id = dataLayer.add(symptom7);
    
    
    ExtendedSymptom symptom8;
    symptom8.id = "";
    symptom8.categoryID = categoryGeneral.id;
    symptom8.difficulty = difficultySomeExplanation;
    symptom8.confirmed = true;
    symptom8.name = "No ping to some sites";
    symptom8.description = "Ping command to some sites fails but to others is working.";
    symptom8.tags.insert("ping");
    symptom8.tags.insert("sites");
    symptom8.steps.push_back("Type 'ping ' followed by the address of a site you know does not work.");
    symptom8.steps.push_back("Type 'ping ' followed by the address of several different international sites.");
    symptom8.steps.push_back("Type 'ping ' followed by the address of several different local sites.");
    symptom8.steps.push_back("Check if any of the pings fail but other succeed");
    
    symptom8.id = dataLayer.add(symptom8);
    
    
    ExtendedSymptom symptom9;
    symptom9.id = "";
    symptom9.categoryID = categoryGeneral.id;
    symptom9.difficulty = difficultyJustByIntuition;
    symptom9.confirmed = true;
    symptom9.name = "Messages from firewall appear";
    symptom9.description = "Messages from firewall appear from time to time.";
    symptom9.tags.insert("firewall");
    symptom9.tags.insert("message");
    symptom9.steps.push_back("Open a site you know does not work.");
    symptom9.steps.push_back("Open several different international sites.");
    symptom9.steps.push_back("Open several different local sites.");
    symptom9.steps.push_back("Check if any firewall message pops in the tray");
    
    symptom9.id = dataLayer.add(symptom9);
    
    
    ExtendedSymptom symptom10;
    symptom10.id = "";
    symptom10.categoryID = categoryWireless.id;
    symptom10.difficulty = difficultyJustByIntuition;
    symptom10.confirmed = true;
    symptom10.name = "No wireless network shows in the list";
    symptom10.description = "Your wireless network is not shown on the list of connections.";
    symptom10.tags.insert("wireless");
    symptom10.tags.insert("network");
    symptom10.tags.insert("list");
    symptom10.steps.push_back("Double-click the wireless network icon in tray.");
    symptom10.steps.push_back("Check if your network name is in the list");
    
    symptom10.id = dataLayer.add(symptom10);
    
    
    ExtendedSymptom symptom11;
    symptom11.id = "";
    symptom11.categoryID = categoryGeneral.id;
    symptom11.difficulty = difficultyJustByIntuition;
    symptom11.confirmed = true;
    symptom11.name = "Network icon missing";
    symptom11.description = "The network icon is not shown in the tray.";
    symptom11.tags.insert("network");
    symptom11.tags.insert("icon");
    symptom11.tags.insert("missing");
    symptom11.steps.push_back("Search the tray for your network icon.");
    symptom11.steps.push_back("Verify that it is missing.");
    
    symptom11.id = dataLayer.add(symptom11);
    
    // Create problems
    // problems
    //    1. lan card hardware problem - 1, 7, 11, 10
    //    2. problem in ISP - 2, 6, 8
    //    3. stuck/off router/switch - 3, 4, 7, 10
    //    4. firewall blocking network - 2, 4, 9
    //    5. site problem - 2, 8
    //    6. router/switch hardware problem - 3, 4, 7, 10
    //    7. incorrect lan / router configuration - 3, 4
    //    8. stuck lan card - 3, 4, 7, 10
    //    9. browser settings - 2, 5
    //    10. wireless not turned on - 3, 4, 11
    //    11. device not installed - 11, 1
    
    
    ExtendedProblem problem1;
    problem1.id = "";
    problem1.categoryID = categoryGeneral.id;
    problem1.difficulty = difficultyModerateExplanation;
    problem1.confirmed = true;
    problem1.name = "Lan / wireless card hardware issue";
    problem1.description = "The lan / wireless card has hardware issues";
    problem1.tags.insert("lan");
    problem1.tags.insert("card");
    problem1.tags.insert("hardware");
    problem1.tags.insert("issue");
    problem1.steps.push_back("Run hardware diagnostic tools.");
    problem1.steps.push_back("Take to technician.");
    problem1.steps.push_back("Temporary replace the card and observe if the symptoms disappear.");
    
    problem1.id = dataLayer.add(problem1);
    
    
    ExtendedProblem problem2;
    problem2.id = "";
    problem2.categoryID = categoryGeneral.id;
    problem2.difficulty = difficultyJustByIntuition;
    problem2.confirmed = true;
    problem2.name = "ISP problem";
    problem2.description = "There is a problem with the service the ISP is delivering";
    problem2.tags.insert("isp");
    problem2.tags.insert("problem");
    problem2.steps.push_back("Call ISP to check if there are problems on their side.");
    
    problem2.id = dataLayer.add(problem2);
    
    
    ExtendedProblem problem3;
    problem3.id = "";
    problem3.categoryID = categoryGeneral.id;
    problem3.difficulty = difficultyJustByLogic;
    problem3.confirmed = true;
    problem3.name = "Stuck/off router/switch";
    problem3.description = "The router/switch is stuck or not powered on";
    problem3.tags.insert("router");
    problem3.tags.insert("switch");
    problem3.tags.insert("stuck");
    problem3.tags.insert("off");
    problem3.steps.push_back("Check if the router is plugged in the power and has lights on.");
    problem3.steps.push_back("Check if the router is showing activity on your connection slot.");
    problem3.steps.push_back("Try restarting the router and see if the symptoms will disappear.");
    
    problem3.id = dataLayer.add(problem3);
    
    
    ExtendedProblem problem4;
    problem4.id = "";
    problem4.categoryID = categoryGeneral.id;
    problem4.difficulty = difficultyModerateExplanation;
    problem4.confirmed = true;
    problem4.name = "Firewall blocking network";
    problem4.description = "The firewall is blocking some or all of the network access";
    problem4.tags.insert("firewall");
    problem4.tags.insert("blocking");
    problem4.tags.insert("network");
    problem4.steps.push_back("Open the firewall settings.");
    problem4.steps.push_back("Check if the firewall is running.");
    problem4.steps.push_back("Check if the firewall has rules regarding the network.");
    problem4.steps.push_back("Check if the firewall has rules regarding sites that you know don't work.");
    
    problem4.id = dataLayer.add(problem4);
    
    
    ExtendedProblem problem5;
    problem5.id = "";
    problem5.categoryID = categoryGeneral.id;
    problem5.difficulty = difficultyJustByLogic;
    problem5.confirmed = true;
    problem5.name = "Website has problem";
    problem5.description = "There is an issue on the website provider side";
    problem5.tags.insert("website");
    problem5.tags.insert("problem");
    problem5.steps.push_back("Contact the owner of the site.");
    problem5.steps.push_back("Check if there is a problem on their side.");
    
    problem5.id = dataLayer.add(problem5);
    
    
    ExtendedProblem problem6;
    problem6.id = "";
    problem6.categoryID = categoryGeneral.id;
    problem6.difficulty = difficultyJustByLogic;
    problem6.confirmed = true;
    problem6.name = "Router/Switch hardware problem";
    problem6.description = "The router / switch has hardware problems";
    problem6.tags.insert("router");
    problem6.tags.insert("switch");
    problem6.tags.insert("hardware");
    problem6.tags.insert("problem");
    problem6.steps.push_back("Check if the device can power up.");
    problem6.steps.push_back("Check if the indicators of the device light up.");
    problem6.steps.push_back("Check if you can connect to the device.");
    
    problem6.id = dataLayer.add(problem6);
    
    
    ExtendedProblem problem7;
    problem7.id = "";
    problem7.categoryID = categoryGeneral.id;
    problem7.difficulty = difficultyModerateExplanation;
    problem7.confirmed = true;
    problem7.name = "Incorrect lan / router configuration";
    problem7.description = "The lan card / router has incorrect IPv4 settings.";
    problem7.tags.insert("lan");
    problem7.tags.insert("router");
    problem7.tags.insert("configuration");
    problem7.steps.push_back("For lan: open the lan card settings and go to ipv4.");
    problem7.steps.push_back("For router: connect to the router and go to the ip configuration page.");
    problem7.steps.push_back("Check if the settings are the same as the ISP requires.");
    
    problem7.id = dataLayer.add(problem7);
    
    
    ExtendedProblem problem8;
    problem8.id = "";
    problem8.categoryID = categoryGeneral.id;
    problem8.difficulty = difficultyJustByLogic;
    problem8.confirmed = true;
    problem8.name = "Lan / wireless card is stuck";
    problem8.description = "The lan / wireless card is not responding and stuck.";
    problem8.tags.insert("lan");
    problem8.tags.insert("wireless");
    problem8.tags.insert("stuck");
    problem8.steps.push_back("Check that you don't have localhost ping.");
    problem8.steps.push_back("Restart the machine.");
    problem8.steps.push_back("Check if the symptoms have disappeared.");
    
    problem8.id = dataLayer.add(problem8);
    
    
    ExtendedProblem problem9;
    problem9.id = "";
    problem9.categoryID = 	.id;
    problem9.difficulty = difficultyJustByLogic;
    problem9.confirmed = true;
    problem9.name = "Restrictive browser settings";
    problem9.description = "The browser settings restrict the access to some sites.";
    problem9.tags.insert("browser");
    problem9.tags.insert("settings");
    problem9.steps.push_back("Open the browser configuration and find the security tab.");
    problem9.steps.push_back("Check that security is either too high or there are specific site restrictions.");
    
    problem9.id = dataLayer.add(problem9);
    
    
    ExtendedProblem problem10;
    problem10.id = "";
    problem10.categoryID = categoryWireless.id;
    problem10.difficulty = difficultyJustByLogic;
    problem10.confirmed = true;
    problem10.name = "Wireless not enabled";
    problem10.description = "The wireless device is not enabled.";
    problem10.tags.insert("wireless");
    problem10.tags.insert("enabled");
    problem10.steps.push_back("Search for an indicator showing the status of wireless and check if it is on.");
    problem10.steps.push_back("Open the network connections and check if the wireless adaptor is disabled.");
    
    problem10.id = dataLayer.add(problem10);
    
    
    ExtendedProblem problem11;
    problem11.id = "";
    problem11.categoryID = categoryGeneral.id;
    problem11.difficulty = difficultySomeExplanation;
    problem11.confirmed = true;
    problem11.name = "Device not installed";
    problem11.description = "The device has not been installed or has been installed incorrectly.";
    problem11.tags.insert("device");
    problem11.tags.insert("installed");
    problem11.steps.push_back("Open the device manager and search for the lan / wireless adaptor.");
    problem11.steps.push_back("Check if it is missing or has exclamation mark.");
    
    problem11.id = dataLayer.add(problem11);
    
    
    // Add solutions
    // solutions
    // 1. replace problematic lan card - 1, 8
    // 2. contact ISP - 2
    // 3. restart router/switch - 3
    // 4. replace problematic router/switch - 3, 6
    // 5. contact website owner if needed - 4
    // 6. enter correct lan / router settings - 7
    // 7. restart computer - 8
    // 8. change browser settings - 9
    // 9. turn on wireless - 10
    // 10. install drivers for the device - 11
    
    ExtendedSolution solution1;
    solution1.id = "";
    solution1.categoryID = categoryGeneral.id;
    solution1.difficulty = difficultyModerateExplanation;
    solution1.confirmed = true;
    solution1.name = "Replace card";
    solution1.description = "Replace problematic lan / wireless card";
    solution1.tags.insert("replace");
    solution1.tags.insert("card");
    solution1.steps.push_back("Open the computer.");
    solution1.steps.push_back("Find the lan card.");
    solution1.steps.push_back("Unmount it and mount a new one on its place.");
    
    solution1.id = dataLayer.add(solution1);
    
    
    ExtendedSolution solution2;
    solution2.id = "";
    solution2.categoryID = categoryGeneral.id;
    solution2.difficulty = difficultyJustByIntuition;
    solution2.confirmed = true;
    solution2.name = "Contact ISP";
    solution2.description = "Contact the ISP provider in order to fix a problem";
    solution2.tags.insert("contact");
    solution2.tags.insert("isp");
    solution2.steps.push_back("Contact the ISP and let them help you resolve a problem.");
    
    solution2.id = dataLayer.add(solution2);
    
    
    ExtendedSolution solution3;
    solution3.id = "";
    solution3.categoryID = categoryGeneral.id;
    solution3.difficulty = difficultyJustByIntuition;
    solution3.confirmed = true;
    solution3.name = "Restart router / switch";
    solution3.description = "Restart the router / switch";
    solution3.tags.insert("restart");
    solution3.tags.insert("router");
    solution3.tags.insert("switch");
    solution3.steps.push_back("Find the router / switch.");
    solution3.steps.push_back("Press the restart button if it has one.");
    solution3.steps.push_back("Unplug the device, wait 10sec and plug it back again.");
    
    solution3.id = dataLayer.add(solution3);
    
    
    ExtendedSolution solution4;
    solution4.id = "";
    solution4.categoryID = categoryGeneral.id;
    solution4.difficulty = difficultyModerateExplanation;
    solution4.confirmed = true;
    solution4.name = "Replace router/switch";
    solution4.description = "Replace problematic router/switch";
    solution4.tags.insert("replace");
    solution4.tags.insert("router");
    solution4.tags.insert("switch");
    solution4.steps.push_back("Find the router / switch.");
    solution4.steps.push_back("Write down the settings and configuration.");
    solution4.steps.push_back("Unplug the device and any cables.");
    solution4.steps.push_back("Plug a new device in the same way as the old one.");
    solution4.steps.push_back("Configure the new device with the same settings as the old one.");
    
    solution4.id = dataLayer.add(solution4);
    
    
    ExtendedSolution solution5;
    solution5.id = "";
    solution5.categoryID = categoryGeneral.id;
    solution5.difficulty = difficultyJustByLogic;
    solution5.confirmed = true;
    solution5.name = "Contact website owner";
    solution5.description = "Contact site owner for information regarding how to fix an issue with the website";
    solution5.tags.insert("contact");
    solution5.tags.insert("website");
    solution5.tags.insert("owner");
    solution5.steps.push_back("Contact site owner for information regarding how to fix an issue with the website.");
    
    solution5.id = dataLayer.add(solution5);
    
    
    ExtendedSolution solution6;
    solution6.id = "";
    solution6.categoryID = categoryGeneral.id;
    solution6.difficulty = difficultySomeExplanation;
    solution6.confirmed = true;
    solution6.name = "Enter lan / router settings";
    solution6.description = "Enter setting for the lan card / router";
    solution6.tags.insert("enter");
    solution6.tags.insert("lan");
    solution6.tags.insert("router");
    solution6.tags.insert("settings");
    solution6.steps.push_back("Open the lan / router configuration window.");
    solution6.steps.push_back("Enter the settings that need to be changed.");
    
    solution6.id = dataLayer.add(solution6);
    
    
    ExtendedSolution solution7;
    solution7.id = "";
    solution7.categoryID = categoryGeneral.id;
    solution7.difficulty = difficultyJustByIntuition;
    solution7.confirmed = true;
    solution7.name = "Restart computer";
    solution7.description = "Restart computer";
    solution7.tags.insert("restart");
    solution7.tags.insert("computer");
    solution7.steps.push_back("Press the power button or turn off the computer via the operating system.");
    solution7.steps.push_back("Wait 10 sec and press the power button again.");
    
    solution7.id = dataLayer.add(solution7);
    
    
    ExtendedSolution solution8;
    solution8.id = "";
    solution8.categoryID = categoryGeneral.id;
    solution8.difficulty = difficultyJustByIntuition;
    solution8.confirmed = true;
    solution8.name = "Change browser settings";
    solution8.description = "Change browser security settings";
    solution8.tags.insert("browser");
    solution8.tags.insert("settings");
    solution8.steps.push_back("Open the browsers settings and find the security tab.");
    solution8.steps.push_back("Remove any blocked content and reduce the security level.");
    
    solution8.id = dataLayer.add(solution8);
    
    
    ExtendedSolution solution9;
    solution9.id = "";
    solution9.categoryID = categoryWireless.id;
    solution9.difficulty = difficultySomeExplanation;
    solution9.confirmed = true;
    solution9.name = "Enable wireless";
    solution9.description = "Turn on wireless networking on the machine";
    solution9.tags.insert("enable");
    solution9.tags.insert("wireless");
    solution9.steps.push_back("Find a button that enables the wireless access on your machine.");
    solution9.steps.push_back("Open network connections and find the wireless adaptor.");
    solution9.steps.push_back("Right click the adaptor and select 'enable'.");
    
    solution9.id = dataLayer.add(solution9);
    
    
    ExtendedSolution solution10;
    solution10.id = "";
    solution10.categoryID = categoryGeneral.id;
    solution10.difficulty = difficultyModerateExplanation;
    solution10.confirmed = true;
    solution10.name = "Install device drivers";
    solution10.description = "Install the device drivers";
    solution10.tags.insert("device");
    solution10.tags.insert("drivers");
    solution10.tags.insert("install");
    solution10.steps.push_back("Open device manager.");
    solution10.steps.push_back("If the device is there, open it and click 'Reinstall driver' and follow the instructions.");
    solution10.steps.push_back("If the device is not there, press the button 'scan for hardware changes'.");
    solution10.steps.push_back("If your device is detected, follow the instructions on screen.");
    
    solution10.id = dataLayer.add(solution10);
    
        /** LAYOUT
     
        // categories
        1. general
        2. lan
        3. wireless


        // symptoms
        1. can't ping localhost
        2. can access only some sites
        3. network stays disconnected
        4. can't ping router/switch
        5. message pops in browser when site doesnt load
        6. no problem can be detected
        7. the lan light between the router/computer back is not on
        8. ping only to specific sites is missing
        9. messages from firewall pop from time to time
        10. can't find wireless network
        11. network icon missing


        // problems
        1. lan card hardware problem - 1, 7, 11, 10
        2. problem in ISP - 2, 6, 8
        3. stuck/off router/switch - 3, 4, 7, 10
        4. firewall blocking network - 2, 4, 9
        5. site problem - 2, 8
        6. router/switch hardware problem - 3, 4, 7, 10
        7. incorrect lan / router configuration - 3, 4
        8. stuck lan card - 3, 4, 7, 10
        9. browser settings - 2, 5
        10. wireless not turned on - 4, 11
        11. device not installed - 11, 1


        // solutions
        1. replace problematic lan card - 1, 8
        2. contact ISP - 2
        3. restart router/switch - 3
        4. replace problematic router/switch - 3, 6
        5. contact site owner if needed - 4
        6. enter correct lan / router settings - 7
        7. restart computer - 8
        8. change browser settings - 9
        9. turn on wireless - 10
        10. install drivers for the device - 11
    */
    
    
    // Add symptom links
    addSymptomLink(symptom1, problem1, 100, 30, 0, dataLayer);
    addSymptomLink(symptom1, problem11, 30, 100, 0, dataLayer);

    addSymptomLink(symptom2, problem2, 50, 50, 50, dataLayer);
    addSymptomLink(symptom2, problem4, 100, 80, 20, dataLayer);
    addSymptomLink(symptom2, problem5, 100, 100, 0, dataLayer);
    addSymptomLink(symptom2, problem9, 100, 60, 20, dataLayer);
    
    addSymptomLink(symptom3, problem3, 100, 20, 10, dataLayer);
    addSymptomLink(symptom3, problem6, 20, 40, 0, dataLayer);
    addSymptomLink(symptom3, problem7, 1000, 200, 100, dataLayer);
    addSymptomLink(symptom3, problem8, 100, 60, 20, dataLayer);
    
    addSymptomLink(symptom4, problem3, 100, 20, 10, dataLayer);
    addSymptomLink(symptom4, problem4, 200, 40, 0, dataLayer);
    addSymptomLink(symptom4, problem6, 100, 30, 10, dataLayer);
    addSymptomLink(symptom4, problem7, 500, 200, 100, dataLayer);
    addSymptomLink(symptom4, problem8, 50, 20, 50, dataLayer);
    addSymptomLink(symptom4, problem10, 300, 150, 0, dataLayer);
    
    addSymptomLink(symptom5, problem9, 300, 0, 0, dataLayer);
    
    addSymptomLink(symptom6, problem2, 100, 0, 0, dataLayer);

    addSymptomLink(symptom7, problem1, 100, 100, 0, dataLayer);
    addSymptomLink(symptom7, problem3, 100, 10, 10, dataLayer);
    addSymptomLink(symptom7, problem6, 100, 50, 0, dataLayer);
    addSymptomLink(symptom7, problem8, 100, 20, 20, dataLayer);
    
    addSymptomLink(symptom8, problem2, 100, 50, 20, dataLayer);
    addSymptomLink(symptom8, problem5, 100, 20, 20, dataLayer);
    
    addSymptomLink(symptom9, problem4, 100, 0, 0, dataLayer);
    
    addSymptomLink(symptom10, problem1, 100, 100, 0, dataLayer);
    addSymptomLink(symptom10, problem3, 100, 10, 10, dataLayer);
    addSymptomLink(symptom10, problem6, 100, 50, 0, dataLayer);
    addSymptomLink(symptom10, problem8, 100, 20, 20, dataLayer);
    
    addSymptomLink(symptom11, problem1, 100, 100, 10, dataLayer);
    addSymptomLink(symptom11, problem10, 100, 10, 0, dataLayer);
    addSymptomLink(symptom11, problem11, 100, 20, 0, dataLayer);
    
    
    // Add solution links
    addSolutionLink(solution1, problem1, 100, 0, dataLayer);
    addSolutionLink(solution1, problem8, 100, 0, dataLayer);
    
    addSolutionLink(solution2, problem2, 100, 0, dataLayer);
    
    addSolutionLink(solution3, problem3, 100, 30, dataLayer);
    
    addSolutionLink(solution4, problem3, 100, 0, dataLayer);
    addSolutionLink(solution4, problem6, 100, 0, dataLayer);
    
    addSolutionLink(solution5, problem4, 100, 0, dataLayer);
    
    addSolutionLink(solution6, problem7, 100, 0, dataLayer);
    
    addSolutionLink(solution7, problem8, 100, 50, dataLayer);
    
    addSolutionLink(solution8, problem9, 100, 0, dataLayer);
    
    addSolutionLink(solution9, problem10, 100, 0, dataLayer);
    
    addSolutionLink(solution10, problem11, 100, 0, dataLayer);
    
    SystemManager manager(new MongoDbDataLayer("localhost:22222", "isp_kb"));
    
    Investigation firstInvestigation;
    firstInvestigation.id = "";
    firstInvestigation.closed = false;
    firstInvestigation.positiveSymptoms.push_back(symptom4.id);
    
    firstInvestigation.id = manager.getDataLayer().add(firstInvestigation);
    
    SolvingMachine::Suggestion suggestion = manager.makeSuggestion(firstInvestigation.id);
    
    printf("Symptoms:\n");
    for(uint i = 0; i < suggestion.symptoms.size(); ++i)
    {
        printf("%s (%d)  -   ", suggestion.symptoms[i].c_str(), suggestion.symptomValues[i]);
    }
    printf("\n");
    
    printf("Problems:\n");
    for(uint i = 0; i < suggestion.problems.size(); ++i)
    {
        printf("%s (%d)  -   ", suggestion.problems[i].c_str(), suggestion.problemValues[i]);
    }
    printf("\n");
    
    printf("Solutions:\n");
    for(uint i = 0; i < suggestion.solutions.size(); ++i)
    {
        printf("%s (%d)  -   ", suggestion.solutions[i].c_str(), suggestion.solutionValues[i]);
    }
    printf("\n");
    
    return 0;
}
