/********************************************************************
* Progammer: SHOBHIT SRIVASTAVA
*
* Purpose:   This is the header file of the Assign4.cc
*********************************************************************/

#include <iostream>
#include <fstream>
#include <cstring>
#include <queue>
#include <string>
#include <ctime>
#include <unistd.h>
#include <sstream>
#include <iomanip>
#include "Process.cc"

//Constant Declarations
#define MAX_TIME 500
#define AT_ONCE 5
#define QUEUE_SIZE 20
#define ARRAY_SIZE 10
#define HOW_OFTEN 25

//Global Variables
Process process;                                //Creating a process
queue<Process> Entry_Queue;                     //Creating a queue to take in all the processes
queue<Process> Terminate_Queue;                 //Creating a queue to take in all the terminated processes
priority_queue<Process> Ready_PQ;               //Creating a priority queue to take in the processe whenever there is a CPU burst
priority_queue<Process> Input_PQ;               //Creating a priority queue to take in the process whenever there is an Input burst
priority_queue<Process> Output_PQ;              //Creating a priority queue to take in the process whenever there is an Output burst
Process *Active, *IActive, *OActive;            //Creating pointers to the processes

//Function prototypes for the required processes

void buildEntryQueue();

void startProcess();
void printProcessStats();

int moveQueueFrom_Active(int,int,int);
void moveQueueFrom_IActive(int);
void moveQueueFrom_OActive(int);

int terminateActive(int,int,int);

void printdata(int, int);
