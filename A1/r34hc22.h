#ifndef R34HC22_H
#define R34HC22_H

#include "micro.h"

namespace mc 
{
    class R34HC22 : public Micro
	{
	private:
		unsigned char a;
		unsigned char b;
	public:
		// constructor
		R34HC22();

		// reset the microcontroller
		void reset();
		
		// get the status string
		string statusString();

		unsigned char getA();
		unsigned char getB();

		void setA(unsigned char);
		void setB(unsigned char);

		// execute the instruction coorecsponding to the opcode
		void execute();

		// execute from a specific location in memory
		void executeFromLocation(int);

		// execute functions
		void moveAToMemory(int);

		void loadRegisterWithValue(int,string);
		
		void incrementRegisterA(int);
		void branchAlways(int);
		void branchIfALessThanB(int);
		void branchIfLessThanA(int);
		void haltOpcode();

		int getLocation(int, int, int);
	};
}

#endif
