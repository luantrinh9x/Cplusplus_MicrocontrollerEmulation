#ifndef PIC32F42_H
#define PIC32F42_H
#include "micro.h"

namespace mc 
{
	class PIC32F42 : public Micro 
	{
	private:
		// special purpose byte-sized register called W
		unsigned char w;
	public:
		// constructor
		PIC32F42();

		// Getters
		unsigned char getW();

		// Setters
		void setW(unsigned char);

		// get the status string
		string statusString();

		// reset microcontroller
		void reset();

		// execute the instruction coorecsponding to the opcode
		void execute();

		// execute from a specific location in memory
		void executeFromLocation(int);

		// execute functions
		void moveValueToW(int);
		void moveWToMemory(int);
		void operationsValueToW(int,string);
		void goToAddress(int);
		void branchIfNotEqual(int);
		void haltOpcode();

		int getLocation(int, int, int);
	};
}

#endif
