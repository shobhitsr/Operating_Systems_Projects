/********************************************************************
* Progammer: SHOBHIT SRIVASTAVA
*
* Purpose:   In this program we simmulate a we are simulating priority
*			 			 scheduling of processes on a single-processor system,
*			 			 without preemption. The main pronciple of the project is
*            that when a process ends its CPU burst (or input burst
*			 			 or output burst), it is succeeded by the highest-priority
*						 process that is waiting.
*********************************************************************/
#include "Schedule.h"

using namespace std;

int main()
{
	//Method for building the Entry Queue
	buildEntryQueue();
	//Method for processing the Entry Queue for the CPU, Input and Output bursts
	startProcess();
}
//Method to process the entry queue and then move around the ready, input, and output priority queues
void startProcess()
{
	//Initiallizing the required variables.
	int Timer = 0;
	int idleTime = 0;
	int ProcessCounter = 0;

	//Checking if the queue is empty
	if(!Entry_Queue.empty())
	{
		//If the process counter is less than AT_ONCE and arrival time is less than the timer
		while(Entry_Queue.front().get_ArrivalTime() <= Timer && ProcessCounter < AT_ONCE)
		{
				ProcessCounter++;			//Increment the process counter
				Entry_Queue.front().set_TimeStamp(Timer);			//Set a time stamp to the process
				Ready_PQ.push(Entry_Queue.front());			//Push the process to the ready queue
				cout << endl <<"Process "<< Entry_Queue.front().get_ProcessID()<<" moved from the Entry Queue into the Ready Queue at time "<< Timer << endl;
				Entry_Queue.pop();			//Pop the top element of the queue
		}
	}

	//Now if the timer is less than the MAX_Time and if the process counter is greater than 0
	while(Timer <= MAX_TIME && ProcessCounter > 0)
	{
		//Now take one element a time if the Process counter is less than AT_ONCE
		if(ProcessCounter < AT_ONCE && !Entry_Queue.empty())
		{
			//Check if the arrival time has been reached
			if(Entry_Queue.front().get_ArrivalTime() <= Timer)
			{
				ProcessCounter++;			//Incrementing the process counter
				Entry_Queue.front().set_TimeStamp(Timer);			//Setting a timestamp to the process
				Ready_PQ.push(Entry_Queue.front());			//Pushing th process to the entry queue
				cout << endl <<"Process "<< Entry_Queue.front().get_ProcessID()<<" moved from the Entry Queue into the Ready Queue at time "<< Timer << endl;
				Entry_Queue.pop();			//Poping the top process from the entry queue
			}

		}
		//If the timer is equal to HOW_OFTEN then print the status of the processes
		if(Timer % HOW_OFTEN == 0)
		{
			cout << endl <<"Status at time " << Timer << endl;
			printProcessStats();			//Call th print process function
		}

		//Check if there is something in the ready queue and if the Active pointer is null
		if(!Ready_PQ.empty() && Active == nullptr)
		{
			Process Active_Process;			//Create a new process
			Active_Process = Ready_PQ.top();			//Put the top of the process in the Active process
			Ready_PQ.pop();			//Pop the top process of the ready queue
			Active = new Process;
			*Active = Active_Process;			//Have the Active pointer point to the Active process
			Active->set_CPUCount(1);			//Increment the CPU counter
			Active->set_CPUTotal(Active->get_HistoryValue());			//Add to the total time of the CPU burst
			Active->set_CPUTimer(Timer + Active->get_HistoryValue());			//Add the current timer to the CPU burst time
		}
		//If the ready queue is empty and the active pointer is not null then wait in the idle state
		else if(Ready_PQ.empty() && Active != nullptr)
		{
			idleTime++;			//Increment the idle counter
		}

		//When the timer reaches the CPU timer then it moves to the next burst
		if(Active != nullptr &&(Active->get_CPUTimer()) == Timer)
		{
			ProcessCounter = moveQueueFrom_Active(ProcessCounter,Timer,idleTime);			//Move from ready queue to either input or output queue
			delete Active;			//Delete the pointer
			Active = nullptr;			//Make the pointer a null pointer
		}

		//Check if there is something in the input queue and if the IActive pointer is null
		if(!Input_PQ.empty() && IActive == nullptr)
		{
			Process IActive_Process;			//Create a new process
			IActive_Process = Input_PQ.top();			//Put the top of the process in the IActive process
			Input_PQ.pop();			//Pop the top process of the input queue
			IActive = new Process;
			*IActive = IActive_Process;			//Have the IActive pointer point to the IActive process
			IActive->set_ICount(1);				//Increment the Input counter
			IActive->set_ITotal(IActive->get_HistoryValue());			//Add to the total time of the Input burst
			IActive->set_IOTimer(Timer + IActive->get_HistoryValue());			//Add the current timer to the Input burst time
		}

		//When the timer reaches the Input timer then it moves to the next burst
		if(IActive != nullptr &&(IActive->get_IOTimer()) == Timer)
		{
			moveQueueFrom_IActive(Timer);			//Move from input queue to either ready or output queue
			delete IActive;			//Delete the pointer
			IActive = nullptr;			//Make the pointer a null pointer
		}

		//Check if there is something in the output queue and if the OActive pointer is null
		if(!Output_PQ.empty() && OActive == nullptr)
		{
			Process OActive_Process;			//Create a new process
			OActive_Process = Output_PQ.top();			//Put the top of the process in the OActive process
			Output_PQ.pop();			//Pop the top process of the output queue
			OActive = new Process;
			*OActive = OActive_Process;			//Have the OActive pointer point to the OActive process
			OActive->set_OCount(1);			//Increment the Output counter
			OActive->set_OTotal(OActive->get_HistoryValue());			//Add to the total time of the Output burst
			OActive->set_IOTimer(Timer + OActive->get_HistoryValue());			//Add the current timer to the Output burst time
		}

		//When the timer reaches the Input timer then it moves to the next burst
		if(OActive != nullptr && (OActive->get_IOTimer()) == Timer)
		{
			moveQueueFrom_OActive(Timer);			//Move from output queue to either input or ready queue
			delete OActive;			//Delete the pointer
			OActive = nullptr;			//Make the pointer a null pointer
		}

		if(Entry_Queue.empty()&&Ready_PQ.empty()&&Input_PQ.empty()&&Output_PQ.empty()&& Active == nullptr && IActive == nullptr && OActive == nullptr)
			break;

		Timer++;			//Increment the timer
	}
	printdata(Timer, idleTime);			//Print the final data after we go out of the loop
}

//Move a process from the ready queue to either input queue or output queue
int moveQueueFrom_Active(int ProcessCounter, int Timer, int idleTime)
{
	Active->increment_Sub();			//Increment the subscript of the history array
	//Check if history letter is I
	if(Active->get_HistoryLetter() == "I")
	{
		Active->set_TimeStamp(Timer);			//Update the time stamp on the process
		Input_PQ.push(*Active);			//Push the process into the input queue
	}
	//Check if the letter is O
	else if (Active->get_HistoryLetter() == "O")
	{
		Active->set_TimeStamp(Timer);			//Update the time stamp on the process
		Output_PQ.push(*Active);			//Push the process into the output queue
	}
	//Check if the letter is N
	else if (Active->get_HistoryLetter() == "N")
	{
		ProcessCounter = terminateActive(ProcessCounter, Timer, idleTime);			//Terminate the process if N has been reached
		return ProcessCounter;			//Return the counter
	}
	return ProcessCounter;			//Return the counter
}

//Move a process from the input queue to either ready queue or output queue
void moveQueueFrom_IActive(int Timer)
{
	IActive->increment_Sub();			//Increment the subscript of the history array
	//Check if history letter is C
	if(IActive->get_HistoryLetter() == "C")
	{
		IActive->set_TimeStamp(Timer);			//Update the time stamp on the process
		Ready_PQ.push(*IActive);			//Push the process into the input queue
	}
	//Check if history letter is O
	else if (IActive->get_HistoryLetter() == "O")
	{
		IActive->set_TimeStamp(Timer);			//Update the time stamp on the process
		Output_PQ.push(*IActive);			//Push the process into the output queue
	}
}
//Move a process from the output queue to either ready queue or input queue
void moveQueueFrom_OActive(int Timer)
{
	OActive->increment_Sub();			//Increment the subscript of the history array
	//Check if history letter is C
	if(OActive->get_HistoryLetter() == "C")
	{
		OActive->set_TimeStamp(Timer);			//Update the time stamp on the process
		Ready_PQ.push(*OActive);			//Push the process into the ready queue
	}
	//Check if history letter is I
	else if (OActive->get_HistoryLetter() == "I")
	{
		OActive->set_TimeStamp(Timer);			//Update the time stamp on the process
		Input_PQ.push(*OActive);			//Push the process into the input queue
	}
}

//Method for terminating the process when it comes to the end of all the bursts
int terminateActive(int ProcessCounter, int Timer, int idleTime)
{
	Terminate_Queue.push(*Active);			//Push the active process to the terminate queue
	ProcessCounter--;			//Decrement the process counter

	//Print out the details about the terminated process
	cout << endl << "A process has terminated." << endl;
	cout << setw(21) << left << "Process ID:" << Active->get_ProcessID() << endl;
	cout << setw(21) << left  << "Name:" << Active->get_ProcessName() << endl;
	cout << setw(21) << left << "Priority:" << Active->get_priority() << endl;
	cout << setw(21) << left << "Started at time" << Active->get_ArrivalTime() << " and ended at time " << Timer << endl;
	cout << setw(21) << left << "Total CPU time =" << Active->get_CPUTotal() << " in " << Active->get_CPUCount() << " CPU bursts" << endl;
	cout << setw(21) << left << "Total Input time =" << Active->get_ITotal() << " in " << Active->get_ICount() << " Input bursts" << endl;
	cout << setw(21) << left << "Total Output time =" << Active->get_OTotal() << " in " << Active->get_OCount()<< " Output bursts" << endl;
	cout << setw(21) << left << "Time spent waiting:" << idleTime << endl;
	return ProcessCounter;			//Return the counter
}

//Method to build the Entry queue
void buildEntryQueue()
{
	ifstream file;			//Create a variable to read the file
    file.open ("data4.txt");			//open the file

    //Check for errors
	if (!file.is_open())
		cout << endl << "Error opening file" << endl;

	//Initialize the required variables
    string word;
	string str;
	int x = 0 , y = 0, index = 0;
	int processID = 101;
	bool CPU = false;
	bool Input = false;
	bool Output =false;
	bool Nullword = false;
	bool Lstrecord = false;

	//While there is a line to read, read the next line
    while(getline(file, str))
	{
		istringstream iss(str);			//Creating a variable to process the string
		//While there is a word in the string, read the word
		while (iss >> word)
		{
			//Deliminator for the last process
			if(word == "STOPHERE"||Lstrecord)
			{
				Lstrecord = true;
				break;
			}
			//Deliminator for the end of process
			if(word == "N")
			{
				process.set_HistoryLetter(word,index);
				Nullword = true;
				continue;
			}
			if(Nullword == true)
			{
				process.set_HistoryValue(0,index);
				Nullword = false;
				index++;
				continue;
			}
			//Check the size to get a new process
			if(word.size() == 8)
			{

				process.set_ProcessName(word);			//Set process name
				process.set_ProcessID(processID);			//Set the process ID
				processID++;			//Increment the processID
				y++;
				continue;

			}
			if(y == 1)
			{
				process.set_priority(stoi(word));			//Setting the priority
				y++;
				continue;
			}
			if(y ==2)
			{
				process.set_ArrivalTime(stoi(word));			//Setting the arrival time
				y = 0;
				continue;
			}
			if(word == "C")
			{
				process.set_HistoryLetter(word,index);			//Put the letter in the history
				CPU = true;
				continue;
			}
			if(CPU == true)
			{
				process.set_HistoryValue(stoi(word),index);			//Put the value in history
				CPU = false;
				index++;
				continue;
			}
			if(word == "I")
			{
				process.set_HistoryLetter(word,index);			//Put the letter in the history
				Input = true;
				continue;
			}
			if(Input == true)
			{
				process.set_HistoryValue(stoi(word),index);			//Put the value in history
				Input = false;
				index++;
				continue;
			}
			if(word == "O")
			{
				process.set_HistoryLetter(word,index);			//Put the letter in the history
				Output = true;
				continue;
			}
			if(Output == true)
			{
				process.set_HistoryValue(stoi(word),index);			//Put the value in history
				Output = false;
				index++;
				continue;
			}
		}
		x++;			//increment the process seperator
		if(x == 2 && Lstrecord == false)
		{
			Entry_Queue.push(process);			//Push the process into the entry queue
			x = 0;
		}
		index = 0;
	}
}

//Method for printing the process statistics with the contents all the queues
void printProcessStats()
{
	if(Active == nullptr)
		cout << "Active is 0" << endl;
	else
		cout << "Active is " << Active->get_ProcessID() << endl;
	if(IActive == nullptr)
		cout << "IActive is 0" << endl;
	else
		cout << "IActive is " << IActive->get_ProcessID() << endl;
	if(OActive == nullptr)
		cout << "OActive is 0" << endl;
	else
		cout << "OActive is " << OActive->get_ProcessID() << endl;

	queue<Process> PrintEntryQueue = Entry_Queue;
	cout << "Contents of the Entry Queue:" << endl << endl;
	while (!PrintEntryQueue.empty())
	{
		cout << "Process ID: " << PrintEntryQueue.front().get_ProcessID() << " Priority = " << PrintEntryQueue.front().get_priority() << " arriving at time = " << PrintEntryQueue.front().get_ArrivalTime() << endl;
		PrintEntryQueue.pop();
	}
	cout << '\n';
	priority_queue<Process> PrintReadyQueue = Ready_PQ;
	cout << "Contents of the Ready Queue" << endl << endl;
	while (!PrintReadyQueue.empty())
	{

		cout << "Process ID: " << PrintReadyQueue.top().get_ProcessID() << " Priority = " << PrintReadyQueue.top().get_priority() << " arriving at time = " << PrintReadyQueue.top().get_ArrivalTime() << endl;
		PrintReadyQueue.pop();
	}
	cout << '\n';

	priority_queue<Process> PrintInputQueue = Input_PQ;
	cout << "Contents of the Input Queue" << endl << endl;
	while (!PrintInputQueue.empty())
	{

		cout << "Process ID: " << PrintInputQueue.top().get_ProcessID() << " Priority = " << PrintInputQueue.top().get_priority() << " arriving at time = " << PrintInputQueue.top().get_ArrivalTime() << endl;
		PrintInputQueue.pop();
	}
	cout << '\n';

	priority_queue<Process> PrintOutputQueue = Output_PQ;
	cout << "Contents of the Output Queue" << endl << endl;
	while (!PrintOutputQueue.empty())
	{

		cout << "Process ID: " << PrintOutputQueue.top().get_ProcessID() << " Priority = " << PrintOutputQueue.top().get_priority() << " arriving at time = " << PrintOutputQueue.top().get_ArrivalTime() << endl;
		PrintOutputQueue.pop();
	}
	cout << '\n';

}
//Method to print the final data after all the processes have run succusfully
void printdata(int Timer, int idleTime)
{
	cout << endl << "The run has ended." << endl;
	cout << "The final value of the timer was:  " << Timer <<endl;
	cout << "The amount of time spent idle was:  " << idleTime << endl;
	cout << "Number of terminated processes =    " << Terminate_Queue.size() << endl;
	printProcessStats();			//Printing the stats of the queues and the pointers.
}
