/********************************************************************
* Progammer: SHOBHIT SRIVASTAVA
*
* Purpose:   The purpose of this file is to make the getters and
*			 setters of the processes and to overload < and >
*********************************************************************/

#include "Process.h"
void set_ArrivalTime(int ArrivalTime);

//The default constructor for a process
Process::Process()
{
    ProcessName = "";
	Priority = 0;
	ProcessID = 0;
	ArrivalTime = 0;
	timestmp = 0;
	History[10];
	Sub = 0;
	CPUTimer = 0;
	IOTimer = 0;
	CPUTotal = 0;
	ITotal = 0;
	OTotal = 0;
	CPUCount = 0;
	ICount = 0;
	OCount = 0;
}

Process::~Process()
{

}
//Making the getters and setters for the different variables of the process
void Process::set_ProcessName(string name)
{
	this->ProcessName = name;
}
string Process::get_ProcessName()const
{
	return this->ProcessName;
}

int Process::get_priority() const
{
	return (this->Priority);
}
void Process::set_priority(int priority_Level)
{
	this->Priority = priority_Level;
}

int Process::get_ProcessID()const
{
	return(this->ProcessID);
}
void Process::set_ProcessID(int ID)
{
	this->ProcessID = ID;
}

int Process::get_ArrivalTime()const
{
	return this->ArrivalTime;
}
void Process::set_ArrivalTime(int Time)
{
	this->ArrivalTime = Time;
}

void Process::set_TimeStamp(int timestp)
{
	this->timestmp = timestp;
}
int Process:: get_TimeStamp() const
{
	return (this->timestmp);
}

void Process::set_HistoryLetter(string L,int index)
{
	this->History[index].letter = L;
}
string Process::get_HistoryLetter()const
{
	return this->History[this->Sub].letter;
}

void Process::set_HistoryValue(int val, int index)
{
	this->History[index].value = val;
}
int Process::get_HistoryValue()const
{
	return this->History[this->Sub].value;
}

void Process::increment_Sub()
{
	this->Sub++;
}
int Process::get_Sub()
{
	return this->Sub;
}
void Process::clear_Sub()
{
	this->Sub = 0;
}

void  Process::set_CPUTimer(int time)
{
	this->CPUTimer =time;
}
int Process::get_CPUTimer()const
{
	return this->CPUTimer;
}

void Process::set_IOTimer(int time)
{
	this->IOTimer = time;
}
int Process::get_IOTimer()const
{
	return this->IOTimer;
}

void Process::set_CPUTotal(int total)
{
	this->CPUTotal += total;
}
int Process::get_CPUTotal()const
{
	return this->CPUTotal;
}

void Process::set_ITotal(int total)
{
	this->ITotal += total;
}
int Process::get_ITotal()const
{
	return this->ITotal;
}

void Process::set_OTotal(int total)
{
	this->OTotal += total;
}
int Process::get_OTotal()const
{
	return this->OTotal;
}

void Process::set_CPUCount(int count)
{
	this->CPUCount += count;
}
int Process::get_CPUCount()const
{
	return this->CPUCount;
}

void Process::set_ICount(int count)
{
	this->ICount += count;
}
int Process::get_ICount()const
{
	return this->ICount;
}

void Process::set_OCount(int count)
{
	this->OCount += count;
}
int Process::get_OCount()const
{
	return this->OCount;
}

//Overloading the < operator for the priority queues
bool Process:: operator<(const Process &P1)const
{
	if (this->get_priority() == P1.get_priority())
		return this->get_TimeStamp() > P1.get_TimeStamp();
	return this->get_priority() < P1.get_priority();
}

//Overloading the > operator for the priority queues
bool Process:: operator>(const Process &P1)const
{
	if (this->get_priority() == P1.get_priority())
		return this->get_TimeStamp() > P1.get_TimeStamp();
	return this->get_priority() < P1.get_priority();
}
