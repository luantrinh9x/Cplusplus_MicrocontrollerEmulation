#ifndef MICRO_H
#define MICRO_H

using std::string;

namespace mc
{
	class Micro
	{
	protected:
		// internal CPU register that point to the memory
		// location that contains the next instruction
		int progCounter;

		// memory of micro
		unsigned char* memory;

		// size of the memory array in bytes
		int memSize;

		// constructor, should be private since no one should be able to create
		// new instance of this class
		Micro(int);
	public:
		
		// destructor
		virtual ~Micro();

		// getters
		int getProgCounter();
		unsigned char* getMemory();
		int getMemSize();

		// setters
		void setProgCounter(int);
		
		// reset micro
		virtual void reset() = 0;

		// execute
		virtual void execute() = 0;

		// execute from specific location
		virtual void executeFromLocation(int) = 0;

		// get the status
		virtual string statusString();

		// get the value at the input address location
		unsigned char getMemoryValueAtLocation(int);

		// set the value at the input address location
		void setMemoryValueAtLocation(int, unsigned char);

		// find memory address
		int findAddress();

		// read a hexadecimal number from user
		int askHexLocation(string);
	};
}

#endif
