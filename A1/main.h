#ifndef MAIN_H
#define MAIN_H

#include "micro.h"

using namespace mc;

// function for handle choices
typedef void choice(Micro**);

// check function
char checkChoice();
string readFromUser(string ask, string ques);
void checkConnected(Micro** mic, choice* execute);

// choices function
void connectToMicro(Micro** mic);
void displayAllMemory(Micro** mic);
void executeCurrentPC(Micro** mic);
void executeSpecificLocation(Micro** mic);
void displayHelp();
void lookAtMemory(Micro** mic);
void modifyMemory(Micro** mic);
void resetMicro(Micro** mic);
void displayStatus(Micro** mic);

// function ask new hex number
unsigned int askHexNew(string ask);

#endif
