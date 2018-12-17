/********************************************************************
* Progammer: SHOBHIT SRIVASTAVA
*
* Purpose:   In this program we simulate a producer consumer problem
*            and we use multithreading with semaphores, threads and
*            mutex to solve this problem. We have multiple producers
*            multiple consumers. THe producer produces the data into
*            the buffer which is then consumed by the consumer.
*********************************************************************/
#include <cstdlib>
#include <pthread.h>
#include <thread>
#include <iostream>
#include <mutex>
#include <semaphore.h>
#include <unistd.h>
#include <sstream>

//Declaration of the constants
#define P_NUMBER 6
#define C_NUMBER 4
#define BUFFER_SIZE 12
#define N_P_STEPS 4
#define N_C_STEPS 6

typedef int buffer_t;                               //Creating a custom data type called buffer_t
pthread_mutex_t mutex;                              //Creating a mutex variable
int count, in, out;                                 //Creating the counter variables
sem_t NotEmpty, NotFull;                            //Creating 2 semaphores to determine NotFull and NotEmpty respectively

//A function to insert the array elements into the buffer.
int Insert(buffer_t *item)
{
    int success;                                    //Value to be returned at the end of the function
    if(sem_wait(&NotFull))                          //Check if the semaphore is NotEmpty
    {
        printf("Error while waiting during insertion");
        exit(-1);                                   //Exit with a conndition code -1
    }
    if(pthread_mutex_lock(&mutex))                  //Lock the mutex variable
    {
        printf("Error while locking the mutex during insertion");
        exit(-1);                                   //Exit with a conndition code -1
    }
    count++;
    printf("Producer %d inserted one item.  Total is now %d\n", *item, count);         //Print the output
    if(pthread_mutex_unlock(&mutex))                //Unlock the mutex variable
    {
        printf("Error while unlocking the mutex during insertion");
        exit(-1);                                   //Exit with a conndition code -1
    }
    if(sem_post(&NotEmpty))                         //Post of the NotEmpty semaphore isn't NotFull
    {
        printf("Error while posting during insertion");
        exit(-1);                                   //Exit with a conndition code -1
    }
    return success;                                 //Return success
}

//A function to remove the array elements from the buffer.
buffer_t Remove(buffer_t *item)
{
    int success;                                    //Value to be returned at the end of the function

    if(sem_wait(&NotEmpty))                         //Check if the semaphore is NotFull
    {
        printf("Error while waiting during removal");
        exit(-1);                                   //Exit with a conndition code -1
    }
    if(pthread_mutex_lock(&mutex))                  //Lock the mutex variable
    {
        printf("Error while locking during removal");
        exit(-1);                                   //Exit with a conndition code -1
    }
    count--;
    printf("Consumer %d removed one item.   Total is now %d\n", *item, count);         //Print the output
    if(pthread_mutex_unlock(&mutex))                //Unlock the mutex variable
    {
        printf("Error while unlocking the mutex during removal");
        exit(-1);                                   //Exit with a conndition code -1
    }
    if(sem_post(&NotFull))                          //Post of the NotFull semaphore isn't NotEmpty
    {
        printf("Error while posting during removal");
        exit(-1);                                   //Exit with a conndition code -1
    }
    return success;                                 //Return success
}

//A function that is called when a producer produces data
//and needs to be inserted into the buffer
void *Produce(void *thread_n)
{
    buffer_t *item = (buffer_t*)thread_n;           //Create a buffer_t variable called item
    int x = 0;                                      //Create a variable to count the number of threads

    //Run the loop till the time we don't reach the set number of threads
    while(x < N_P_STEPS)
    {
        sleep(1);                                   //Sleep for one second
        //Try to insert the item into the buffer
        if(Insert(item))
            printf("Error occured\n");              //Print error message if not successfull
        x++;                                        //Increment the thread counter
    }

    pthread_exit(NULL);                             //Exit the thread after its work is done
}

//A function that is called when a consumer tries to consume data
//and needs to be remove an item from the buffer
void *Consume(void *thread_n)
{
    buffer_t *item = (buffer_t*)thread_n;           //Create a buffer_t variable called item
    int y = 0;                                      //Create a variable to count the number of threads

    //Run the loop till the time we don't reach the set number of threads
    while(y < N_C_STEPS)
    {
        sleep(1);                                   //Sleep for one second
        //Try to remove the item from the buffer
        if(Remove(item))
            printf("Error occured\n");              //Print error message if not successfull
        y++;                                        //Increment the thread counter
    }

    pthread_exit(NULL);                             //Exit the thread after its work is done
}

int main()
{
    int i;                                          //Create a counter for the loops
    if(pthread_mutex_init(&mutex, NULL))            //Initialize the mutex
    {
        printf("Error initializing the mutex");
        exit(-1);                                   //Exit with a conndition code -1
    }
    if(sem_init(&NotFull,0,BUFFER_SIZE))            //Initialize the semaphore NotFull to the BUFFER_SIZE
    {
        printf("Error initializing the Not Full semaphore");
        exit(-1);                                   //Exit with a conndition code -1
    }
    if(sem_init(&NotEmpty,0,0))                     //Initialize the semaphore NotEmpty to 0
    {
        printf("Error initializing the Not Empty semaphore");
        exit(-1);                                   //Exit with a conndition code -1
    }
    count = in = out = 0;                           //Set all the counter variables to 0

    pthread_t producers[P_NUMBER];                  //Create an array of producer threads
    pthread_t consumers[C_NUMBER];                  //Create an array of consumer threads
    buffer_t id[P_NUMBER];
    for(i = 0;i < P_NUMBER;i++)
    {
        id[i] = i;
    }
    //Run the loop for all the producers
    for(i = 0; i < P_NUMBER; i++)
    {
        if(pthread_create(&producers[i], NULL, Produce,(void*) &id[i]))     //Create a thread and call the produce function with the producer data as a variable
        {
            printf("Error creating a producer thread");
            exit(-1);                               //Exit with a conndition code -1
        }
    }

    //Run the loop for all the consumers
    for(i = 0; i < C_NUMBER; i++)
    {
        if(pthread_create(&consumers[i], NULL, Consume,(void*) &id[i]))     //Create a thread and call the consume function with the condumer data as a variable
        {
            printf("Error creating a consumer thread");
            exit(-1);                               //Exit with a conndition code -1
        }
    }

    //Run the loop for all the producers
    for (i = 0; i < P_NUMBER; i++)
    {
        if(pthread_join(producers[i], NULL))        //Join all the producer threads
        {
            printf("Error joining the producer threads");
            exit(-1);                               //Exit with a conndition code -1
        }
    }

    //Run the loop for all the consumers
    for (i = 0; i < C_NUMBER; i++)
    {
        if(pthread_join(consumers[i], NULL))        //Join all the consumer threads
        {
            printf("Error joining the consumer threads");
            exit(-1);                               //Exit with a conndition code -1
        }
    }

    printf("\nAll the producer and consumer threads have been closed.\n");      //Display message after closing all the threads

    if(pthread_mutex_destroy(&mutex))               //Destroy the mutex
    {
        printf("Error destroying the mutex");
        exit(-1);                                   //Exit with a conndition code -1
    }
    if(sem_destroy(&NotFull))                          //Destroy the semaphore variable NotFull
    {
        printf("Error destroying the semaphore NotFull");
        exit(-1);                                   //Exit with a conndition code -1
    }
    if(sem_destroy(&NotEmpty))                         //Destroy the semaphore variable NotEmpty
    {
        printf("Error destroying the semaphore NotEmpty");
        exit(-1);                                   //Exit with a conndition code -1
    }

    printf("\nThe semaphores and mutex have been deleted.\n");                  //Display message after destroying th semaphores and the mutex
    return 0;	                                    //Return
}
