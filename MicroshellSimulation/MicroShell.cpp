/********************************************************************
* Progammer: SHOBHIT SRIVASTAVA
*
* Purpose:   In this program we create a microshell with the ability to
*	    			 use a pipe. We have a pipe symbolized by a '||'. The
* 	     		 program uses pipes, forks, dup, execvp function.
*********************************************************************/


#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <sys/utsname.h>
#include <unistd.h>
#define arr_size 1024

using namespace std;

int main(void)
{
	char buf[1024];						//Variable to take in the user input


	pid_t pid;							//PID for the first child process
	pid_t pid2;							//PID for the second child process

	int  status;						//Variable to check the status of the first child
	int  status2;						//Variable to check the status of the second child

	int pipeA[2];						//Creating a read and write array for the pipe

	cout<< "shell> ";					//Prompt to ask the user for input

	while (fgets(buf,arr_size,stdin) != NULL) 		//Continiue till the user inputs q or quit
	{

		bool pipeExists = false;			//A boolean vaue to check if the pipe exsists.

		//initialize this arrays to NULL so anything that store in them gets cleared out.
		//these arrays will hold the commands that the user wants to carry out.
		char * command[arr_size] = {NULL, NULL, NULL, NULL, NULL, NULL};	//Takes up to 5 arguements
		char * command2[arr_size] = {NULL, NULL, NULL, NULL, NULL, NULL};	//Take up to 5 arguements

		buf[strlen(buf) -1] = 0;			//Delete mark the last byte as 0

		int index = 0;					//Index counter

		char * char_ptr;				//Character pointer to hold the saved by strtok
		char_ptr = strtok(buf, " ");

		while(char_ptr != NULL)				//Check Buffer and save token in it correct place
		{

			if(strcmp( char_ptr, "q" ) == 0 || strcmp(char_ptr,"quit") == 0)	//Check if q or quit has been entered
			{
				exit(0);
			}

			if(strcmp( char_ptr, "||" ) == 0)				//Check if pipe exists
			{
				pipeExists = true;					//Set pipeExists to True
				index= 0;						//Set index to 0
				char_ptr = strtok(NULL, " ");				//Get the next character
			}

			if(!pipeExists)							//If pipe doesn't exisit
			{
				command[index] = char_ptr;				//Put character in the command array
				char_ptr = strtok(NULL, " ");				//Get the next character
				index++;						//Increment the index pointer
			}

			if(pipeExists)							//If the pipe exists
			{
				command2[index] = char_ptr;				//Put the next char in command2 array
				char_ptr = strtok(NULL, " ");				//Get the next character
				index++;						//Increment the index pointer
			}
		}

		if(pipeExists)								//If the pipe exists
		{
			pipe(pipeA);							//Create a pipe
		}

		if ((pid = fork()) == 0) 						//Create the first child
		{
			if(pipeExists)							//If the pipe exists
			{								//Check if the pipe exsists
				dup2(pipeA[1], 1);					//Replacing std output with output part of pipe
				close(pipeA[0]);					//Close the read end of the pipe
				close(pipeA[1]);					//Close the write end of the pipe
			}

			execvp(command[0], command);					//Execute the command at command[0]
			printf("Invalid first command. \n");				//Print an error message if the command isn't executable
			exit(127);							//Exit with condition 127
		}
		else if(pid < 0)							//Handle errors if error exists
		{
			cerr<<"error forking first child"<<endl;			//Print an error message
		}

		if(pipeExists)								//If the pipe exists
		{
			if ((pid2 = fork()) == 0) 					//Create the second child
			{
				dup2(pipeA[0], 0);					//Replacing std input with input part of pipe
				close(pipeA[1]);					//Close the write end of the pipe
				close(pipeA[0]);					//Close the read end of the pipe
				execvp(command2[0], command2);				//Execute the command at command2[0]
				printf("Invalid Piped Command \n");			//Print an error message if the command isn't executable
				exit(127);						//Exit with condition 127
			}
			else if(pid2 < 0)						//Handle error if error exists
			{
				cerr<<"error forking second child"<<endl;		//Print error message
			}
		}

		if(pipeExists)								//If the pipe exists
		{
			close(pipeA[0]);						//Close the read end of the pipe
			close(pipeA[1]);						//Close the write end of the pipe
		}

		if ( (pid = waitpid(pid, &status, 0)) < 0)				//Wait for the first child to finish executing
		{
			cerr<<"error waiting for first child"<<endl;			//Print the error message
		}

		if(pipeExists)								//If the pipe exists
		{
			if ( (pid2 = waitpid(pid2, &status2, 0)) < 0)			//Wait for the seconf child to finish execution
			{
				cerr<<"error waiting for second child"<<endl;		//Print the error message
			}
		}

		cerr<<"shell> ";							//Print the prompt again

	  }
	  exit(0);									//Exit with the condition 0
}
