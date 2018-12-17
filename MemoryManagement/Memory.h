/********************************************************************
* Progammer: SHOBHIT SRIVASTAVA
*
* Purpose:   This is the header file of the class file Memory.h
*********************************************************************/

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

//Class defination for Memory block
class Memory
{
	int StartAddress;
	int size;
	string processID;
	string blockID;

	public:

	//Function protoypes of the memory block
	Memory();
	Memory(int, int, string, string);
	
	int getStartAddress()const;
	void setStartAddress(int);

	int getsize()const;
	void setsize(int);
	
	string getprocessID()const;
	void setprocessID(string);
	
	string getblockID()const;
	void setblockID(string);

	bool operator==(const Memory&)const;
	bool operator==(const string& PID) const;
	bool operator>=(const Memory&)const;
};
