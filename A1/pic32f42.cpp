#include <iostream>
#include <sstream>
#include <iomanip>
#include "pic32f42.h"

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::hex;
using std::stringstream;

#define error_mess "\nSIGWEED. Program executed past top of memory"
#define complete_mess "\nProgram executed"
#define invalid_mem "\nInvalid memory address"

namespace mc 
{
	// Constructor
	PIC32F42::PIC32F42() : Micro(1536){
		// print message after init R500
		cout << "\nPIC32F42 selected" << endl;
		// Reset micro PIC32F42
		reset();
	}

	string PIC32F42::statusString(){
		stringstream stream;
		stream << "Current Program Counter location: " << hex << getProgCounter() << endl;
		stream << "W register value: " << hex << (int)getW();
		return stream.str();
	}

	// Reset micro PIC32F42
	void PIC32F42::reset(){
		progCounter = 0;
		setW(0);
		cout << "Microcontroller reset" << endl;
	}

	// Getters
	unsigned char PIC32F42::getW(){
		return w;
	}

	// Setters
	void PIC32F42::setW(unsigned char w){
		this->w = w;
	}

	// execute the instruction coorecsponding to the opcode
	void PIC32F42::execute(){
		executeFromLocation(getProgCounter());
	}

	// execute from a specific location in memory
	void PIC32F42::executeFromLocation(int address)
	{
		// get the opcode in memory
		unsigned char opcode = getMemoryValueAtLocation(address);
		switch(opcode)
		{
			case 0x50:				
				moveValueToW(address);
				break;
			case 0x51:				
				moveWToMemory(address);
				break;
			case 0x5A:				
				operationsValueToW(address,"add");
				break;
			case 0x5B:				
				operationsValueToW(address,"sub");
				break;
			case 0x6E:			
				goToAddress(address);
				break;
			case 0x70:				
				branchIfNotEqual(address);
				break;
			case 0xFF:				
				haltOpcode();
				break;
			default:				
				cerr << "SIGOP. Invalid opcode. Program Counter = " << hex << getProgCounter() << endl;
				break;
		}
	}

	// 0x50
	// Move Value to W
	void PIC32F42::moveValueToW(int address)
	{
		if((address + 2) < getMemSize())
		{
			// set the new value for W
			setW(getMemoryValueAtLocation(address + 1));
			setProgCounter(address + 2);
			executeFromLocation(address + 2);
			cout << complete_mess << endl;
		} else {
			cerr << error_mess << endl;
			haltOpcode();
		}
	}

	// 0x51
	// Move W to memory
	void PIC32F42::moveWToMemory(int address)
	{
		if((address + 3) < getMemSize())
		{
			if(getLocation(address,1,2) != -1)
			{
				setMemoryValueAtLocation(getLocation(address,1,2), getW());
				setProgCounter(address + 3);
				executeFromLocation(address + 3);
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

	// 0x5A
	// Add Value to W
	void PIC32F42::operationsValueToW(int address, string operation)
	{
		if((address + 2) < getMemSize())
		{
			if(operation.compare("add") == 0)
			{
				// add value to W
				setW(getW() + getMemoryValueAtLocation(address + 1));
			}
			else if(operation.compare("sub") == 0)
			{
				setW(getW() - getMemoryValueAtLocation(address + 1));
			}
			setProgCounter(address + 2);
			executeFromLocation(address + 2);
			cout << complete_mess << endl;
		} else {
			cerr << error_mess << endl;
			haltOpcode();
		}
	}

	// 0x6E
	// Go to address (branch always)
	void PIC32F42::goToAddress(int address)
	{
		if((address + 2) < getMemSize())
		{
			if(getLocation(address,1,2) != -1)
			{
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

	// 0x70
	// Branch if not equal
	void PIC32F42::branchIfNotEqual(int address){

		if((address + 4) < getMemSize())
		{
			unsigned char compareValue = getMemoryValueAtLocation(address + 1);
			// compare value and W
			if(compareValue == getW())
			{
				setProgCounter(address + 4);
				executeFromLocation(address + 4);
				cout << complete_mess << endl;
			} else {
				if(getLocation(address,2,3) != -1)
				{
					setProgCounter(getLocation(address,2,3));
					executeFromLocation(getLocation(address,2,3));
					cout << complete_mess << endl;
				} else {
					cerr << invalid_mem << endl;
					haltOpcode();
				}
			}
		} else {
			cerr << error_mess << endl;
			haltOpcode();
		}
	}

	// 0xFF
	// Halt opcode
	void PIC32F42::haltOpcode(){
		cout << "\nProgram halted" << endl;
	}

	// return the location of given memory address 
	int PIC32F42::getLocation(int address, int highNo, int lowNo)
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
