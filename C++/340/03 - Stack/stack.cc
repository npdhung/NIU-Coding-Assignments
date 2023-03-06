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
#include "stack.h"

bool Stack::empty() const
{
    return (q1.empty() && q2.empty());
}

int Stack::size() const
{
    return static_cast<int>(q1.size() + q2.size());
}

int Stack::top()
{
    if (!q1.empty()) return q1.back();
    else return q2.back();
}

void Stack::push(const int &val)
{
    if (!q1.empty()) q1.push(val);
    else if (!q2.empty()) q2.push(val);
        else q1.push(val);
}

void Stack::pop()
{
    if (!q1.empty() && q2.empty()) {
        size_t size = q1.size();
        for (size_t i=0; i<(size-1); ++i) {
            int temp = q1.front();
            q2.push(temp);
            q1.pop();
        }
        q1.pop();
    }
    else if (q1.empty() && !q2.empty()) {
        size_t size = q2.size();
        for (size_t i=0; i<(size-1); ++i) {
            int temp = q2.front();
            q1.push(temp);
            q2.pop();
        }
        q2.pop();
    }
}
