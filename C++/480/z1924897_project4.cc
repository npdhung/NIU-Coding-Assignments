/*******************************************************************************
CSCI480 - Assignment 4 - Spring 2023
Readers-Writers using POSIX API

Programmer: Hung Nguyen (Z1924897)
Section:    0001
TA:         Jake Rogers
Due Date:   04/11/2023

Purpose:    Solve the Reader-Writer problem using the PThreads library.
*******************************************************************************/
#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

using namespace std;
string shared_string = "All work and no play makes Jack a dull boy.";
sem_t write_sem;    // writer semaphore
sem_t read_sem;     // reader semaphore

int read_count = 0;

void *writer(void *threadid)
{
    long tid;
    tid = (long)threadid;
    
    while (shared_string.length() > 0)
    {
        sem_wait(&write_sem);
        if (shared_string.length() == 0) 
        {
            sem_post(&write_sem);
            break;
        }
        shared_string.pop_back();
        cout << "writer " << tid << " is writing ..." << endl;
        sem_post(&write_sem);
        sleep(1);
    }
    // use write_sem as a mutex lock to protect the display
    sem_wait(&write_sem);
    cout << "writer " << tid << " is exiting ..." << endl;
    sem_post(&write_sem);
    pthread_exit(NULL);
}

void *reader(void *threadid)
{
    long tid;
    tid = (long)threadid;
   
    while (shared_string.length() > 0)
    {
        sem_wait(&read_sem);    // acquire lock, enter the critical section
        if (shared_string.length() == 0) 
        {
            sem_post(&read_sem);
            break;
        }
        read_count++;
        cout << "read_count increments to: " << read_count << "." << endl;
        if (read_count == 1)    // first reader, then ask writer to wait
            sem_wait(&write_sem);
        
        // perform reading
        if (shared_string.length() > 0)
            cout << "reader "<< tid << " is reading ... "
                << "content : " << shared_string << endl;
        sem_post(&read_sem);    // release lock, exit the critical section
        // other readers can enter
        sem_wait(&read_sem);    // done reading, acquire lock
        read_count--;
        cout << "read_count decrements to: " << read_count << "." << endl;
        if (shared_string.length() == 0) 
        {
            sem_post(&write_sem);
            sem_post(&read_sem);
            break;
        }
        if (read_count == 0)    // nobody is reading, release the wrt semaphore
            sem_post(&write_sem);
        sem_post(&read_sem);    // release lock, exit the critical section
    }
    sem_wait(&write_sem);
    cout << "reader " << tid << " is exiting ..." << endl;
    sem_post(&write_sem);
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    // Get command line argument
    if (argc < 3)
    {
        cout << "Please enter two arguments. Exit!" << endl;
        return 0;
    }  
    int rd_num = atoi(argv[1]); // number of reader threads
    int wr_num = atoi(argv[2]); // number of writer threads

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
    sem_init(&write_sem,0,1);
    sem_init(&read_sem,0,1);

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
    sem_destroy(&read_sem);
    sem_destroy(&write_sem);
    cout << "Resources cleaned up." << endl;
    return 0;
}

