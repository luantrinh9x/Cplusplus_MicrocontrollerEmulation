#include <iostream>
#include <iomanip>
#include <sstream>
#include "r34hc22.h"

using std::cout;
using std::cerr;
using std::endl;
using std::hex;
using std::stringstream;

#define error_mess "\nSIGWEED. Program executed past top of memory"
#define complete_mess "\nProgram executed"
#define invalid_mem "\nInvalid memory address"

namespace mc 
{
	// Constructor
	R34HC22::R34HC22() : Micro(512)
	{
		cout << "\n34HC22 selected" << endl;
		reset();
	}
	
	// get the status string
	string R34HC22::statusString(){
		stringstream stream;
		stream << "Current Program Counter location: " << hex << getProgCounter() << endl;
		stream << "A register value: " << hex << (int)getA() << endl;
		stream << "B register value: " << hex << (int)getB();
		return stream.str();
	}

	void R34HC22::reset()
	{
		setProgCounter(509);
		cout << "Microcontroller reset" << endl;
	}

	// Getters
	unsigned char R34HC22::getA(){
		return a;
	}

	unsigned char R34HC22::getB(){
		return b;
	}

	// Setters
	void R34HC22::setA(unsigned char a){
		this->a = a;
	}
	
	void R34HC22::setB(unsigned char b){
		this->b = b;
	}

	// execute the instruction coorecsponding to the opcode
	void R34HC22::execute(){
		executeFromLocation(getProgCounter());
	}

	// execute from a specific location in memory
	void R34HC22::executeFromLocation(int address)
	{
		// get the opcode
		unsigned char opcode = getMemoryValueAtLocation(address);

		switch(opcode)
		{
			case 0x0C:				
				moveAToMemory(address);
				break;
			case 0x37:				
				loadRegisterWithValue(address,"A");
				break;
			case 0x38:				
				loadRegisterWithValue(address,"B");
				break;
			case 0x53:				
				incrementRegisterA(address);
				break;
			case 0x5A:			
				branchAlways(address);
				break;
			case 0x5B:				
				branchIfALessThanB(address);
				break;
			case 0x5D:				
				branchIfLessThanA(address);
				break;
			case 0x64:				
				haltOpcode();
				break;
			default:			
				cerr << "SIGOP. Invalid opcode. Program Counter = " << hex << getProgCounter() << endl;
				break;
		}
	}

	// 0x0C
	// Move A to memory
	void R34HC22::moveAToMemory(int address)
	{
		if((address + 3) < getMemSize())
		{
			if(getLocation(address,1,2) != -1)
			{
				// move the content of A to the destination address in memory
				setMemoryValueAtLocation(getLocation(address,1,2), getA());

				// set the program counter to the third byte after the opcode
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

	// 0x37
	// Load A with Value
	void R34HC22::loadRegisterWithValue(int address, string r)
	{
		if((address + 2) < getMemSize())
		{
			if(r.compare("A") == 0)
			{
				// load value from the first byte after the opcode to A
				setA(getMemoryValueAtLocation(address + 1));
			}
			else if(r.compare("B") == 0)
			{
				// load value from the first byte after the opcode to B
				setB(getMemoryValueAtLocation(address + 1));
			}
			// set the program counter the the second byte after opcode
			setProgCounter(address + 2);
			executeFromLocation(address + 2);
			cout << complete_mess << endl;
		} else {
			cerr << error_mess << endl;
			haltOpcode();
		}
	}

	// 0x53
	// Increment Register A
	void R34HC22::incrementRegisterA(int address){

		if((address + 1) < getMemSize()){
			// increare the value of A
			setA(getA() + 1);

			// move the program counter to the next byte
			setProgCounter(address + 1);
			executeFromLocation(address + 1);
			cout << complete_mess << endl;
		} else {
			cerr << error_mess << endl;
			haltOpcode();
		}
	}

	// 0x5A
	// Branch Always
	void R34HC22::branchAlways(int address)
	{
		if((address + 2) < getMemSize())
		{
			if(getLocation(address,1,2) != -1)
			{
				// set the program counter to the new address
				setProgCounter(getLocation(address,1,2));
				executeFromLocation(getLocation(address,1,2));
				cout << complete_mess << endl;
			}
			else
			{
				cerr << invalid_mem << endl;
				haltOpcode();
			}
		} else {
			cerr << error_mess << endl;
			haltOpcode();
		}
	}

	// 0x5B
	// Branch if A < B
	void R34HC22::branchIfALessThanB(int address)
	{
		if((address + 3) < getMemSize())
		{
			// compare the value of A and B
			if(getA() < getB())
			{
				if(getLocation(address,1,2) != -1)
				{
					// set the program counter to the new address
					setProgCounter(getLocation(address,1,2));
					executeFromLocation(getLocation(address,1,2));
					cout << complete_mess << endl;
				} else {
					cout << invalid_mem << endl;
					haltOpcode();
				}
			} else {
				// set the program counter to the third byte after the opcode
				setProgCounter(address + 3);
				executeFromLocation(address + 3);
				cout << complete_mess << endl;
			}
		} else {
			cerr << error_mess << endl;
			haltOpcode();
		}
	}

	// 0x5D
	// Branch if Less than A
	void R34HC22::branchIfLessThanA(int address)
	{
		if((address + 4) < getMemSize())
		{
			// get the value to compare to A
			unsigned char compareValue = getMemoryValueAtLocation(address + 1);

			if(compareValue < getA())
			{
				if(getLocation(address,2,3) != -1)
				{
					// set the program counter to the new address
					setProgCounter(getLocation(address,2,3));
					executeFromLocation(getLocation(address,2,3));
					cout << complete_mess << endl;
				} else {
					cout << invalid_mem << endl;
					haltOpcode();
				}
			} else {
				// set the program counter to the the fourth byte after the opcode
				setProgCounter(address + 4);
				executeFromLocation(address + 4);
				cout << complete_mess << endl;
			}
		} else {
			cerr << error_mess << endl;
			haltOpcode();
		}
	}

	// 0x64
	// Halt opcode
	void R34HC22::haltOpcode()
	{
		cout << "\nProgram halted" << endl;
	}

	// return the location of given memory address 
	int R34HC22::getLocation(int address, int highNo, int lowNo)
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
