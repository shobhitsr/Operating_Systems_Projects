/********************************************************************

Progammer: SHOBHIT SRIVASTAVA

Purpose:   This program displays the process ID of the child and the
	   			 parent of a fork. It also helps understand the functions
					 of the wait(), sleep(), system(), and the ps commands.
Run:	 		
					 make SystemFunction
					 ./SystemFunction
*********************************************************************/

#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
using namespace std;

int main()
{
	int pid,pid2; //Initializing the variables to store the PID
	
	cout << "I am the original process.  My PID is  " << getpid() << " and my parent's PID is " << getppid() << endl; // printing the original process and the PIDs
	
	cout << "Now we have the first fork." << endl;
	
	pid = fork(); //Creating a fork
	
	if( pid == 0 )
	{
		cout << "I am the child.  My PID is  " << getpid() << " and my parent's PID is  " << getppid() << endl; //Displaying the child's PID and then it parent's PID
		
		cout << "Now we have the second fork." << endl;
		
		pid2 = fork(); //Creating a second fork
		
		if ( pid2 == 0 )
		{
			cout << "I am the grandchild.  My PID is  " << getpid() << " and my parent's PID is  " << getppid() << endl; //Displaying the grandchild's PID
			cout << "I am the grandchild, about to exit." << endl;
			exit(0); //Exiting with the condition 0
		}
		else if ( pid2 > 0 )
		{
			cout << "I am still the child.  My PID is  " << getpid() << " and my parent's PID is  " << getppid() << endl; //Displaying the child's PID
			wait(0); //Waiting for the grandchild's process to end.
			cout << "I am the child, about to exit." << endl;
			exit(0); //Exiting with the condition 0
		}
		else
		{
			cout << "The second fork failed." << endl;
			exit(-1); //Incase the second fork fails exit with a condition -1
		}
	}
	else if( pid > 0)
	{
		cout << "I am the parent.  My PID is  " << getpid() << " and my parent's PID is  " << getppid() << endl; //Display the parent's PID and then its parent's PID
		sleep(2); //Make parent sleep for 2 seconds
		cout << "I am the parent, about to call ps." << endl;
		system("ps"); //Call to see all the processes that ran
		wait(0); //Wait for the child to finish execution
		cout << "I am the parent, about to exit." << endl;
		exit(0); //Exiting with a condition 0
	}
	else
	{
		cout << "The first fork failed." << endl;
		exit(-1); //Incase the first fork fails exit with the condition -1
	}
		
	return 0;
}
