/*******************************************************************************
CSCI480 - Assignment 4 - Spring 2023
Readers-Writers using POSIX API

Programmer: Hung Nguyen (Z1924897)
Section:    0001
TA:         Jake Rogers
Due Date:   04/11/2023

Purpose:    Solve the Reader-Writer problem using the PThreads library.
            Part 2 for extra credit.
*******************************************************************************/
#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

using namespace std;
string shared_string = "All work and no play makes Jack a dull boy.";
sem_t first_sem;    // writer semaphore
sem_t second_sem;   // reader semaphore

int wrote_last_line = 0;
int rd_num = 1;     // number of reader threads
int wr_num = 0;     // number of writer threads
void *writer(void *threadid)
{
    long tid;
    tid = (long)threadid;
    
    while (1)
    {
        sem_wait(&first_sem);
        if (shared_string.length() == 0) 
        {
            sem_post(&first_sem);
            break;
        }
        shared_string.pop_back();
        cout << "writer " << tid << " is writing ..." << endl;
        sem_post(&second_sem);
    }
    
    sem_wait(&second_sem);
    cout << "writer " << tid << " is exiting ..." << endl;
    // only write the below line one time
    if (wrote_last_line == 0)
    {
        cout << "There are still " << wr_num - rd_num << " writers waiting " 
            << "on the semaphore..." << endl;
        wrote_last_line++;
    }
    sem_post(&second_sem);
    pthread_exit(NULL);
}

void *reader(void *threadid)
{
    long tid;
    tid = (long)threadid;
   
    while (shared_string.length() > 0)
    {
        sem_wait(&second_sem);    // acquire lock, enter the critical section
        if (shared_string.length() == 0) 
        {
            sem_post(&second_sem);
            break;
        }
        // perform reading
        cout << "reader "<< tid << " is reading ... ";
        cout << "content : " << shared_string << endl;
        sem_post(&first_sem);
        sleep(1);
    }
    // signal the rest writers
    for (int i = 0;i < (wr_num - rd_num);i++) sem_post(&first_sem);
    
    cout << "reader " << tid << " is exiting ..." << endl;
    pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
    // Get command line argument
    if ((argc < 3)||(atoi(argv[1]) != 1))
    {
        cout << "First argument: " << argv[1] <<", second argument: " << argv[2]
            << endl; 
        cout << "Please enter two arguments with first argument is 1. Exit!" 
            << endl;
        return 0;
    }  
    rd_num = atoi(argv[1]);
    wr_num = atoi(argv[2]);

    // print the introduction
    cout << "*** Reader-Writer Problem Simulation ***" << endl;
    cout << "Number of reader threads: " << rd_num << endl;
    cout << "Number of writer threads: " << wr_num << endl;

    // declare threads
    pthread_t wr_threads[wr_num];
    pthread_t rd_threads[rd_num];
    int rc;     // return code
    long t;     // thread id

    // create the writer semaphore and initialize it to 1
    sem_init(&first_sem,0,1);
    sem_init(&second_sem,0,0);

    // Create reader and writer threads
    for(t=0; t<rd_num; t++)
    {
        rc = pthread_create(&rd_threads[t], NULL, reader, (void *)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
      }
    }
    for(t=0; t<wr_num; t++)
    {
        rc = pthread_create(&wr_threads[t], NULL, writer, (void *)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
      }
    }

    // Wait for threads to finish
    for (t=0; t< rd_num; t++)
          pthread_join(rd_threads[t], NULL);
    for (t=0; t< wr_num; t++)
          pthread_join(wr_threads[t], NULL);
    cout << "All threads are done." << endl;
    
    // Cleanup and exit
    sem_destroy(&second_sem);
    sem_destroy(&first_sem);
    cout << "Resources cleaned up." << endl;
    return 0;
}

