#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "micro.h"

using std::cout;
using std::cin;
using std::endl;
using std::cerr;
using std::hex;
using std::string;
using std::stringstream;

namespace mc 
{
	// Constructor
	Micro::Micro(int memSize)
	{
		// init the memory size
		this->memSize = memSize;

		// init the memory array on heap
		memory = new unsigned char [memSize];

		//init the values of memory array to 0
		for(int i = 0; i < memSize; i++){
			memory[i] = 0;
		}
	}

	// Destructor
	Micro::~Micro(){
		delete [] memory;
	}

	// Getters
	unsigned char* Micro::getMemory(){
		return memory;
	}
	int Micro::getMemSize(){
		return memSize;
	}
	int Micro::getProgCounter(){
		return progCounter;
	}

	// Setter
	void Micro::setProgCounter(int progCounter){
		this->progCounter = progCounter;
	}

	// get the value at the input address location
	unsigned char Micro::getMemoryValueAtLocation(int location){
		if(location < 0 || location >= getMemSize()){
			return 0;
		} else {
			return getMemory()[location];
		}
	}

	// set the value at the input address location
	void Micro::setMemoryValueAtLocation(int location, unsigned char value){
		if(location < 0 || location >= getMemSize()){
			cout << "\nCant set value of this address!" << endl;
		} else {
			getMemory()[location] = value;
		}
	}

	// ask user to input a memory location and then return the memory
	// location if that address is exist, else return -1
	int Micro::findAddress(){

		// Init variables here
		// The input location
		int address = askHexLocation("location? ");

		// Validate that the address is out of memory array index
		if(address < 0 || address >= getMemSize()){
			// Display the error for user
			cerr << "\nInvalid address!" << endl;
			return -1;
		} else {
			// Address valid, return the address
			return address;
		}
	}

	// read a hexadecimal number from user
	int Micro::askHexLocation(string ask)
	{
		int value;
		cout << ask;
		// validate hex
		while(!(cin >> hex >> value)){
			cerr << "Please input a valid hex integer!" << endl;
			cin.clear();
			cin.ignore(1000, '\n');
			cout << ask;
		}
		return value;
	}

	// get the status string
	string Micro::statusString()
	{
		stringstream stream;
		stream << "Current Program Counter location: " << hex << getProgCounter();
		return stream.str();
	}
}
