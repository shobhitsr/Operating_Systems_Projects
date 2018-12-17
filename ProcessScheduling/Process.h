/********************************************************************
* Progammer: SHOBHIT SRIVASTAVA
*
* Purpose:   This is the header file of the Process.cc
*********************************************************************/

#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;

//Creating a struct for the history of the bursts of a process
struct event
{
	string letter;
	int value;
};

//Creating a class called process that holds all the variables of the processes that are being processed
class Process
{
	public:
	Process();
	~Process();
	//The getters and setters for processes
	void set_ProcessName(string);
	string get_ProcessName()const;

	int get_priority()const;
	void set_priority(int);

	int get_ProcessID()const;
	void set_ProcessID(pid_t);

	int get_ArrivalTime()const;
	void set_ArrivalTime(int);

	void set_TimeStamp(int);
	int get_TimeStamp() const;

	void set_HistoryLetter(string,int);
	string get_HistoryLetter()const;

	void set_HistoryValue(int, int);
	int get_HistoryValue()const;

	void increment_Sub();
	int get_Sub();
	void clear_Sub();

	void set_CPUTimer(int);
	int get_CPUTimer()const;

	void set_IOTimer(int);
	int get_IOTimer()const;

	void set_CPUTotal(int);
	int get_CPUTotal()const;

	void set_ITotal(int);
	int get_ITotal()const;

	void set_OTotal(int);
	int get_OTotal()const;

	void set_CPUCount(int);
	int get_CPUCount()const;

	void set_ICount(int);
	int get_ICount()const;

	void set_OCount(int);
	int get_OCount()const;

	//opreatorr <  and > overload
	bool  operator<(const Process&) const;
	bool  operator>(const Process&) const;

	private:
	//Declaring all the variables privetely
	string ProcessName;
	int Priority;
	int ProcessID;
	int ArrivalTime;
	int timestmp;
	event History[10];
	int Sub;
	int CPUTimer;
	int IOTimer;
	int CPUTotal;
	int ITotal;
	int OTotal;
	int CPUCount;
	int ICount;
	int OCount;
};
