/********************************************************************
* Progammer: SHOBHIT SRIVASTAVA
*
* Purpose:   This is the header file of the program MemManagement
*********************************************************************/

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include "Memory.cc"

using namespace std;

//Declaring the constants
#define KB 1024
#define MB 1048576
#define HOWOFTEN 5

//Creating the lists of memory blocks
list<Memory> Inuse;
list<Memory> Avail;

//Function prototypes for the program

list<Memory> populateAvail(list<Memory>);

void printLists(list<Memory>, list<Memory>);
void printAvailList(list<Memory>);
void printInuseList(list<Memory>);

void best_fit(ifstream&);
void first_fit(ifstream&);

bool loadTransaction(string,string,int);
bool loadFirstTransaction(string,string,int);

bool allocateTransaction(string,string,int);
bool allocateFirstTransaction(string,string,int);

bool terminateProcess(string);
bool deallocateAllBlock(string);

bool deallocateBlock(string, string);
