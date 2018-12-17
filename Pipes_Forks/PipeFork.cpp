/********************************************************************

Progammer: SHOBHIT SRIVASTAVA

Purpose:   This program works with pipes and forks. The purpose of
	   			 this program is to understand how to pass data between
	   			 different ends of the pipe and between the forks created
	   			 in this program.

Run:
			     make PipeFork
					 ./PipeFork
*********************************************************************/

#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <string.h>
#include <sys/wait.h>

//Defining the constant variables
#define size 15
#define max 99999999

using namespace std;

//Funtion prototypes
void PWork(int [], int []);
void CWork(int [], int []);
void GWork(int [], int []);
void calculateM(char []);
void printvalue(string, char []);

//Funtion for initalizing the first variable value
//then calulating the value of M and then write to pipe A
void PWork(int pipeC[], int pipeA[])
{
	//Initializing the value to 1
	char value[size] = "1";
	char buffer;
	int i = 0;
	string parent = "Parent: ";
	//Running the loop until max value is reached
	do
	{
		printvalue(parent, value);					//Printing the Parent
		write(pipeA[1], value, strlen(value) + 1);	//Writing the value to pipe A

		read(pipeC[0],&buffer,1);					//Reading the one byte from pipe C
		//Running the loop to build a string one byte at a time from the buffer
		do
		{
			value[i] = buffer;						//Appending the value of the buffer to the string
			i++;									//Incrementing the index counter
		} while(read(pipeC[0],&buffer,1) && buffer != '\0');	//Reading from pipe C and checking to see if the deliminator has been reached
		value[i] = buffer;							//Appending the deliminator
		i = 0;										//Reseting the value of the index counter
		calculateM(value);							//Performing the arithmatics
	}while(atoi(value) < max);
	write(pipeA[1],value,strlen(value) + 1);		//Writing the value to pipe A
	close(pipeC[0]);								//Closing the read end of pipe C
	close(pipeA[1]);								//Closing the write end of pipe A

}

//Function to read and write into the pipe for the child process
void CWork(int pipeA[], int pipeB[])
{
	//Initializing the value
	char value[size];
	char buffer;
	int j = 0;
	string child = "Child: ";
	//Running the loop until max value is reached
	do
	{
		read(pipeA[0],&buffer,1);					//Reading the values from pipe A
		//Running the loop to build a string one byte at a time from the buffer
		do
		{
			value[j] = buffer;
			j++;
		}while(read(pipeA[0],&buffer,1) && buffer != '\0');
		value[j] = buffer;

		calculateM(value);							//performing the arithmatics
		printvalue(child, value);				//printing the Child


		write(pipeB[1], value, strlen(value) + 1);	//writing the value to pipe B
		j = 0;
	}while(atoi(value) < max);
	write(pipeB[1],value,strlen(value) + 1);		//writing the value to pipe B
	close(pipeA[0]);								//closing the read end of pipe A
	close(pipeB[1]);								//closing the write end of pipe B
}

//Function to read and write into the pipe for the grandchild process
void GWork(int pipeB[], int pipeC[])
{
	//Initializing the value
	char value[size];
	char buffer;
	int j = 0;
	string Grandchild = "Grandchild: ";
	//running the loop until max value is reached
	do
	{
		read(pipeB[0],&buffer,1);					//reading the values from pipe B

		do
		{
			value[j] = buffer;
			j++;
		}while(read(pipeB[0],&buffer,1) && buffer != '\0');
		value[j] = buffer;

		calculateM(value);							//performing the arithmatics
		printvalue(Grandchild, value);			//printing the Grandchild


		write(pipeC[1], value, strlen(value) + 1);	//writing the value to pipe C
		j = 0;
	}while(atoi(value) < max);
	write(pipeC[1],value,strlen(value) + 1);		//writing the value to pipe C
	close(pipeB[0]);								//closing the read end of pipe B
	close(pipeC[1]);								//closing the write end of pipe C
}

//Function to calculate the value
//of M for passing to the next pipe
void calculateM(char value[])
{
	int M = 1;										//Initializing the M to 1
	M = atoi(value);								//Convert the value to integer
	M = 4 * M + 3;									//Performing the arithmetic
	sprintf(value, "%d", M);						//Converting M back to character value
}

//Funtion to print values of the process and its name
void printvalue(string member, char value[])
{
	cout << setw(15) << left << member << setw(10) << left << " Value = " << value << endl;
}
int main()
{
	pid_t pid1,pid2;					//Creating process Id's
	int pipeA[2], pipeB[2],pipeC[2];	//Creating read and write for pipe A, B, C

	//Checking if there is an error in pipe A
	if(pipe(pipeA) < 0)
	{
		perror("pipe #1 error");
		exit(-5);						//Exiting with a condition -5
	}

	//Checking if there is an error in pipe B
	if(pipe(pipeB) < 0)
	{
		perror("pipe #2 error");
		exit(-5);						//Exiting with a condition -5
	}

	//Checking if there is an error in pipe C
	if(pipe(pipeC) < 0)
	{
		perror("pipe #3 error");
		exit(-5);						//Exiting with a condition -5
	}

	pid1 = fork();						//Creating a fork

	if(pid1 == 0)
	{
		//Child Process
		pid2 = fork();					//Creating another fork
		if(pid2 == 0)
		{
			//GrandChild Process
			close(pipeB[1]);			//Closing the write end of pipe B
			close(pipeC[0]);			//Closing the read end of pipe C
			GWork(pipeB,pipeC);			//Calling the grandchild function
			exit(0);					//Exiting with a condition 0
		}
		else if(pid2 > 0)
		{
			//Child Process
			close(pipeA[1]);			//Closing the write end of pipe A
			close(pipeB[0]);			//Closing the read end of pipe B
			CWork(pipeA, pipeB);		//Calling the child function
			wait(0);					//Waiting for the grandchild function to end
			exit(0);					//Exiting with a condition 0
		}
		else
		{
			//Printing an error message if the second frok fails
			perror("fork #2 error");
			exit(-5);					//Exiting with a condition -5
		}
	}
	else if(pid1 > 0)
	{
		//Parent Process
		close(pipeC[1]);				//Closing the write end of pipe C
		close(pipeA[0]);				//Closing the read end of pipe A
		PWork(pipeC, pipeA);			//Calling the parent funtion
		wait(0);
		exit(0);						//Exiting with a condition 0
	}
	else
	{
		//Printing an error message if the first frok fails
		perror("fork #1 error");
		exit(-5);						//Exiting with a condition -5
	}
}
