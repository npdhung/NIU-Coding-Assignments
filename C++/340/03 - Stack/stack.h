//***************************************************************************
//
//  Hung Nguyen
//  Z1924897
//  CSCI340 - 0002
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//***************************************************************************
#ifndef STACK_H
#define STACK_H
#include <queue>

class Stack
{
private:
    std::queue<int> q1, q2;

public:
    
    /**
     * Check if both queues are empty
     *
     * @return True if both q1 and q2 are empty.
     *
     *********************************************************************************/
    bool empty() const;
    
    /**
     * Count the  number of elements in q1 and q2
     *
     * @return The total number of elements in both queues.
     *
     *********************************************************************************/
    int size() const;
    
    /**
     * Return the newest element.
     *
     * If q1 is not empty, return the end element of q1. Otherwise q2 is not
     * empty and return the end element of q2.
     *
     * @return The newest element.
     *
     *********************************************************************************/
    int top();
    
    /**
     * Add element to a non empty queue
     *
     * Add element to a non empty queue between the two queues. If both
     * queues are empty, the new element is added to the first queue or q1.
     *
     * @param val Value of the element will be added.
     *
     *********************************************************************************/
    void push(const int &val);
    
    /**
     * Remove the newest element.
     *
     * Dump all elements except the newest from the source queue (the queue
     * that contains all elements), to the other queue. Then remove the last (or
     * the newest) element in the source queue.
     *
     *********************************************************************************/
    void pop();
};

#endif // STACK_H
