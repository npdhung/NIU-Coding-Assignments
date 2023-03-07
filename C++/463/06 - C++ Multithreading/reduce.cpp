//******************************************************************************
//
//  CSCI463 - Assignment 6
//
//  Author: Hung Nguyen (Z1924897)
//
//  C++ Multithreading
//
//******************************************************************************
#include <iostream>
#include <unistd.h>
#include <thread>
#include <vector>
#include <mutex> 

using std::cerr;
using std::cout;
using std::endl;

constexpr int rows = 1000;  /// < the num_threadsber of rows in the work matrix
constexpr int cols = 100;   /// < the num_threadsber of cols in the work matrix

std::mutex stdout_lock;             /// < for serializing access to stdout

std::mutex counter_lock;            /// < for dynamic balancing only
volatile int counter = rows;        /// < for dynamic balancing only

std::vector <int> tcount;           /// < count of rows summed for each thread
std::vector <uint64_t> sum;         /// < the calculated sum from each thread

int work[rows][cols];               /// < the matrix to be summed

/**
 * Verify the command-line. 
 * Print an "Usage" error message and terminates the program if it doesn't meet 
 * input requirements. 
 ******************************************************************************/
static void usage()
{
	cerr << "Usage: reduce [-d] [-t num]" << endl;
	cerr << "    -d Use dynamic load-balancing." << endl;
	cerr << "    -t num Specifies the number of threads to use." << endl;
	exit(1);
}

/**
 * Sum the rows of the matrix using dynamic load balancing. 
 * @param tid Thread id number.
 ******************************************************************************/
void sum_dynamic(int tid)
{
    stdout_lock.lock();
    std::cout << "Thread " << tid << " starting" << std::endl;
    stdout_lock.unlock();
    bool done = false;
    while (!done)
    {
        int count_copy;

        counter_lock.lock();
        {
            if (counter > 0) --counter;
            else done = true;
            count_copy = counter;
        }
        counter_lock.unlock();

        if (!done)
        {
            // Do work using the value of count_copy to determine what to do.
            for (int i = 0; i < cols; ++i)
                sum.at(tid) += work[count_copy][i]; // Add to sum
            // Count the num_threadsber of work units that this thread has done
            ++tcount.at(tid);
        }
    }
    stdout_lock.lock();
    std::cout << "Thread " << tid << " ending tcount=" << tcount.at(tid) 
      << " sum=" << sum.at(tid) << std::endl;
    stdout_lock.unlock();
}

/**
 * Sum the rows of the matrix using static load balancing. 
 * @param tid Thread id number.
 * @param num_threads Number of threads will be running.
 ******************************************************************************/
void sum_static(int tid, int num_threads)
{
    stdout_lock.lock();
    std::cout << "Thread " << tid << " starting" << std::endl;
    stdout_lock.unlock();
    int work_row;   // the row will be working on
    bool done = false;
    while (!done)
    {
        // Divide the work by the number of thread
        work_row = tid + tcount.at(tid) * num_threads;
        if (work_row > rows - 1) done = true;

        if (!done)
        {
            for (int i = 0; i < cols; ++i)
                sum.at(tid) += work[work_row][i];   // Add to sum
            // Count the num_threadsber of work units that this thread has done
            ++tcount.at(tid);
        }
    }
    
    stdout_lock.lock();
    std::cout << "Thread " << tid << " ending tcount=" << tcount.at(tid) 
      << " sum=" << sum.at(tid) << std::endl;
    stdout_lock.unlock();
}

/**
 * Sum elements of a matrix using dynamic or static load balancing based on 
 * command-line parameters. 
 * @param argc Number of arguments passed to the program.
 * @param argv Argument vector, each string will be one of the arguments. 
 ******************************************************************************/
int main(int argc, char** argv)
{
    int opt;
    unsigned int num_threads = 2;
    int d_flag = 0;

    while ((opt = getopt(argc, argv, "dt:")) != -1)
    {
        switch (opt)
        {
            case 'd':   d_flag = 1; break;
            case 't':
            {
                num_threads = atoi(optarg);
                if (num_threads > std::thread::hardware_concurrency())
                    num_threads = std::thread::hardware_concurrency();
                break;
            }
            case '?': 
            default: usage();
        }
    }
    
    std::cout << std::thread::hardware_concurrency() 
      << " concurrent threads supported." << std::endl;
    
    std::vector <std::thread*> threads;
    // Make the tcount vector (total count) and sum vector have one element for 
    // each thread & initialize them to 0
    tcount.resize(num_threads, 0);
    sum.resize(num_threads, 0);

    srand(0x1234);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            work[i][j] = rand();
    }

    if (d_flag)
        for (unsigned int i = 0; i < num_threads; ++i)
            threads.push_back(new std::thread(sum_dynamic, i));
    else
        for (unsigned int i = 0; i < num_threads; ++i)
            threads.push_back(new std::thread(sum_static, i, num_threads));

    int total_work = 0;
    uint64_t gross_sum = 0;
    for (unsigned int i = 0; i < num_threads; ++i)
    {
        threads.at(i)->join();  // Wait for thread i to end
        delete threads.at(i);
        total_work += tcount.at(i);
        gross_sum += sum.at(i);
    }    

    std::cout << "main() exiting, total_work=" << total_work << " gross_sum=" 
      << gross_sum << std::endl;
    return 0;
}