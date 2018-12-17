/********************************************************************
* Progammer: SHOBHIT SRIVASTAVA
*
* Purpose:   This is the class file of the program Assign 6
*********************************************************************/


#include "Memory.h"

//Creating a default constructor of the memory class
Memory :: Memory()
{
	StartAddress = 0;
	size = 0;
	processID = "";
	blockID = "";
}

//Creating a constructor of the memory class
Memory ::Memory(int Start, int s, string PID, string BID)
{
	this -> StartAddress = Start;
	this -> size = s;
	this -> processID = PID;
	this -> blockID = BID;
}

//Getter method to get the start address
int Memory :: getStartAddress() const
{
	return this->StartAddress;
}
//Setter method to set the start address
void Memory :: setStartAddress(int Start)
{
	this -> StartAddress = Start;
}

//Getter method to get the size
int Memory :: getsize()const
{
	return this -> size;
}
//Setter method to set the size
void Memory :: setsize(int s)
{
	this -> size = s;
}

//Getter method to get the Process ID
string Memory :: getprocessID()const
{
	return this -> processID;
}
//Setter method to set the Process ID
void Memory :: setprocessID(string PID)
{
	this -> processID = PID;
}

//Getter method to get the Block ID
string Memory :: getblockID()const
{
	return this -> blockID;
}
//Setter method to set the Block ID
void Memory :: setblockID(string BID)
{
	this -> blockID = BID;
}

//Overloading the == opearator to compare size
bool Memory :: operator==(const Memory& zero) const
{
	if(this -> size == zero.getsize())
		return true;
	return false;
}

//overloading the == operator to compare Process ID
bool Memory :: operator==(const string& PID) const
{
	if(this -> processID == PID)
		return true;
	return false;
}

//overloading the >= operator tp compare the start address
bool Memory :: operator>=(const Memory& Terminate)const
{
	if(this -> getStartAddress() >= Terminate.getStartAddress())
		return true;
	return false;
}
