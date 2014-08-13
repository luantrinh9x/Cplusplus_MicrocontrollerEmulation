#include <iostream>
#include <iomanip>

#include "main.h"
#include "micro.h"
#include "mopsr500.h"
#include "pic32f42.h"
#include "r34hc22.h"

using std::cout;
using std::cin;
using std::cerr;
using std::hex;
using std::endl;
using std::string;
using std::setfill;
using std::setw;
using std::stringstream;
using std::tolower;
using namespace mc;

int main()
{
	//
    Micro* mic = 0;
	char choice;
	cout << "\nMICROCONTROLLER EMULATION" << endl;

	do
	{
		cout << "\nEnter an option" << endl;
		cout << "> ";
		choice = checkChoice();
		cout << endl;

        switch (choice)
        {
            case 'c':
                connectToMicro(&mic);
                break;
            case 'd':
                checkConnected(&mic, displayAllMemory);
                break;
            case 'e':
                checkConnected(&mic, executeCurrentPC);
                break;
            case 'g':
                checkConnected(&mic, executeSpecificLocation);
                break;
            case 'h':
                displayHelp();
                break;
            case 'l':
                checkConnected(&mic, lookAtMemory);
                break;
            case 'm':
                checkConnected(&mic, modifyMemory);
                break;
            case 'r':
                checkConnected(&mic, resetMicro);
                break;
            case 's':
                checkConnected(&mic, displayStatus);
                break;
            case 'q':
                cout << "Exit the program..." << endl;
                break;
            default:
                cerr << "Incorrect choice!" << endl;
                break;
        }
    } while (choice != 'q');

    return 0;
}

string readFromUser(string ask, string ques)
{
	string input;
	if(ques != "")
	{
		cout << ques << endl;
	}
	if(ask != "")
	{
		cout << ask;
	}
	cin >> input;
	return input;
}

char checkChoice()
{	
    string ask = readFromUser("","");

    if(ask.length() > 1)
    {
        return '!';
    } else {
        return tolower(ask[0]);
    }
}

void displayHelp()
{
	cout << "c - connect to microcontroller" << endl;
	cout << "d - display all memory" << endl;
	cout << "e - execute from current PC" << endl;
	cout << "g - execution from a specific location" << endl;
	cout << "h - display help" << endl;
	cout << "l - look at memory" << endl;
	cout << "m - modify memory" << endl;
	cout << "r - reset microcontroller" << endl;
	cout << "s - display PC and registers" << endl;
	cout << "q - quit the program" << endl;
}

void connectToMicro(Micro** mic)
{
	// read from user
	string ask = readFromUser("type? ", "Enter a type of micro");

	if(ask.compare("R500") == 0)
	{
		delete (*mic);
		(*mic) = new MopsR500();
	}
	else if(ask.compare("PIC32F42") == 0)
	{
		delete (*mic);
		(*mic) = new PIC32F42();
	}
	else if(ask.compare("34HC22") == 0)
	{
		delete (*mic);
		(*mic) = new R34HC22();
	}
	else
	{
		cerr << "\nInvalid Micro type" << endl;
		cout << "Valid micro types are R500 PIC32F42 34HC22" << endl;
	}
}

void displayAllMemory(mc::Micro** mic)
{
	cout << "\t   ";
	for (int i = 0; i < 16; ++i)
	{
		cout << setw(1) << " " << hex << i << " ";
		if(i == 7)
		{
			cout << "  ";
		}
	}
	cout << endl;

	// loop through the memory array and display
	for (int i = 0; i < (*mic)->getMemSize(); ++i)
	{
		// 16 columns
		if((i % 16) == 0)
		{
			cout << endl << setfill('0') << setw(4) << hex << i << "\t";
		}

		if(i % 8 == 0)
		{
			cout << "  ";
		}
		// print the value in memory
		cout << " " << setfill('0') << setw(2) << hex << (int)(*mic)->getMemory()[i];
	}
	cout << endl;
}

void executeCurrentPC(mc::Micro** mic)
{
	cout << "Execute from current PC" << endl;
	// execute the opcode
	(*mic)->execute();
}

void executeSpecificLocation(mc::Micro** mic)
{
	cout << "Enter the memory location to execute (hex format)" << endl;
	// execute the opcode from location
	(*mic)->executeFromLocation((int)(*mic)->askHexLocation("location? "));
}

// ask the user to input an address and output the value at that address
void lookAtMemory(mc::Micro** mic)
{
	cout << "Enter the memory address to look at (hex format)" << endl;
	
	// input memory address
	int address = (*mic)->findAddress();

	// check if the input address is valid
	if(address != -1)
	{
		cout << "The value at address " << hex << address << " is " <<
		(unsigned int)(*mic)->getMemoryValueAtLocation(address) << endl;
	}
}

void modifyMemory(mc::Micro** mic)
{
	// display the message for user
	cout << "Enter the memory address to modify (hex format)" << endl;

	// input address from user
	int address = (*mic)->findAddress();

	// check if the address is valid
	if(address != -1)
	{
		// print the old value
		cout << "Old value: " << (unsigned int)((*mic)->getMemoryValueAtLocation(address)) << endl;

		// print the message
		cout << "Enter a new value (hex format)" << endl;

		// As each memory address is a byte size (unsigned char), only the
		// lower 8 bits of input should be stored.
		(*mic)->setMemoryValueAtLocation(address, (unsigned char)((unsigned int)askHexNew("new? ")));
	}
}

// reset micro
void resetMicro(mc::Micro** mic)
{
	(*mic)->reset();
}

void displayStatus(mc::Micro** mic) 
{
	cout << (*mic)->statusString() << endl;
}

void checkConnected(mc::Micro** mic, choice* execute)
{
	if((*mic) == 0) 
	{
        // Not connected
		cerr << "Please connect to a controller before excute this operation!" << endl;
	} else {
        // Connected
		execute(mic);
	}
}

unsigned int askHexNew(string ask)
{
	unsigned int value;
	cout << ask;
	// validate input
	while(!(cin >> hex >> value))
	{
		cerr << "Please input a valid hex integer!" << endl;
		cin.clear();
		cin.ignore(1000, '\n');
		cout << ask;
	}
	return value;
}
