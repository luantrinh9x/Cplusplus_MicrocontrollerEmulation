#ifndef MOPSR500_H
#define MOPSR500_H

#include "micro.h"

namespace mc {
	
    class MopsR500 : public Micro
	{
	public:
		// constructor
		MopsR500();
		// reset the micro
		void reset();

		// execute the instruction coorecsponding to the opcode
		void execute();

		// execute from a specific location in memory
		void executeFromLocation(int);

		// functions to execute opcode
		void operationToMemory(int, string);
		void goToAddress(int);
		void branchRelative(int);
		void haltOpcode();

		// get result from low & high byte
		int getLocation(int, int, int);
	};
}

#endif
