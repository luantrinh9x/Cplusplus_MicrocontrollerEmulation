#include <iostream>
#include "mopsr500.h"

using std::cout;
using std::cerr;
using std::endl;
using std::hex;

#define error_mess "\nSIGWEED. Program executed past top of memory"
#define complete_mess "\nProgram executed"
#define invalid_mem "\nInvalid memory address"

namespace mc {

	// Constructor
	MopsR500::MopsR500() : Micro(1024){
		// print message after init R500
		cout << "\nR500 selected" << endl;
		// Reset micro MopsR500
		reset();
	}

	// Reset micro MopsR500
	void MopsR500::reset(){
		// set prog counter to 0
		progCounter = 0;

		// Reinit memory value to 0
		for(int i = 0; i < memSize; i++){
			memory[i] = 0;
		}
		// print reset message
		cout << "Microcontroller reset" << endl;
	}

	// execute the instruction coorecsponding to the opcode
	void MopsR500::execute(){
		executeFromLocation(getProgCounter());
	}

	// execute from a specific location in memory
	void MopsR500::executeFromLocation(int address)
	{
		unsigned char opcode = getMemoryValueAtLocation(address);
		switch(opcode)
		{
			//add
			case 0x0A:				
				operationToMemory(address, "add");
				break;
			//sub
			case 0x13:				
				operationToMemory(address, "sub");
				break;
			//go to address
			case 0x16:				
				goToAddress(address);
				break;
			//branch relative
			case 0x17:				
				branchRelative(address);
				break;
			//halt opcode
			case 0xFF:				
				haltOpcode();
				break;
			//invalid opcode
			default:				
				cerr << "SIGOP. Invalid opcode. Program Counter = " << hex << getProgCounter() << endl;
				break;
		}
	}

	// 0x0A & 0x13
	void MopsR500::operationToMemory(int address, string operation)
	{
		// check if the memory is not out of range
		if((address + 4) < getMemSize()){
			// get the value to add
			unsigned char value = getMemoryValueAtLocation(address + 1);

			if(getLocation(address,2,3) != -1)
			{
				if(operation.compare("add") == 0)
				{
					// add the value to destination
					setMemoryValueAtLocation(getLocation(address,2,3),
						getMemoryValueAtLocation(getLocation(address,2,3)) + value);
				}
				else if(operation.compare("sub") == 0)
				{
					// sub the value to destination
					setMemoryValueAtLocation(getLocation(address,2,3),
						getMemoryValueAtLocation(getLocation(address,2,3)) - value);
				}
				setProgCounter(address + 4);
				executeFromLocation(address + 4);
				cout << complete_mess << endl;
			} else {
				cerr << invalid_mem << endl;
				haltOpcode();
			}
		} else {
			cerr << error_mess << endl;
			haltOpcode();
		}
	}

	// 0x16
	// Go to address
	void MopsR500::goToAddress(int address)
	{
		if((address + 2) < getMemSize())
		{
			// check if the address is valid
			if(getLocation(address,1,2) != -1)
			{
				// set the program counter point to the new address
				setProgCounter(getLocation(address,1,2));
				executeFromLocation(getLocation(address,1,2));
				cout << complete_mess << endl;
			} else {
				cerr << invalid_mem << endl;
				haltOpcode();
			}
		} else {
			cerr << error_mess << endl;
			haltOpcode();
		}
	}

	// 0x17
	// Branch relative
	void MopsR500::branchRelative(int address)
	{
		if((address + 1) < getMemSize())
		{
			// get the value from the next byte after the opcode and then add
			// convert it to signed int
			int value = (signed char)(getMemoryValueAtLocation(address + 1));

			if((address + value) >=0 && (address + value) < getMemSize())
			{
				// move the program counter to the new location
				setProgCounter(address + value);
				executeFromLocation(address + value);
				cout << complete_mess << endl;
			} else {
				cerr << invalid_mem << endl;
				haltOpcode();
			}
		} else {
			cerr << error_mess << endl;
			haltOpcode();
		}
	}

	// 0xFF
	// Halt opcode
	void MopsR500::haltOpcode()
	{
		cout << "\nProgram halted" << endl;
	}

	// return the location of given memory address 
	int MopsR500::getLocation(int address, int highNo, int lowNo)
	{
		unsigned char highByte = getMemoryValueAtLocation(address + highNo);
		unsigned char lowByte = getMemoryValueAtLocation(address + lowNo);

		if(((highByte << 8) | lowByte) >= 0 && ((highByte << 8) | lowByte) < getMemSize())
		{
			return ((highByte << 8) | lowByte);
		}
		return -1;
	}
}
