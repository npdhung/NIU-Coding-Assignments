### CSCI 340 - Data Structures and Algorithm Analysis

**Data Structure:** Stack <BR>
**Programming Focus:** STL Queue

# Stack

For this computer assignment, you are to implement the Stack class using STL queues. All relevant files are located within this GitHub repository. 

*stack.h* contains the definition of the Stack class. It is given here to facilitate the following description:

```c++
class Stack {
    private:
        std::queue<int> q1, q2;
    public:
        bool 	empty() const;
        int 	size() const;
        int 	top();
        void 	push(const int& val);
        void 	pop();
};
```
You are required to implement this class in *stack.cc*. The main function is already provided in *stackmain.cc* and does not need to be modified, you can test your program with *stack.in* and check that it matches *stack.out*.

In the implementation of the class, you are going to use queues `q1` and `q2` to store and manipulate data. You are suggested to keep all elements in either one of the queues. More details are described below.

* `empty()` You need to make sure both `q1` and `q2` are empty.

* `size()` You need to count the number of elements in both `q1` and `q2`.

* `top()` This method returns the newest element. If `q1` is not empty, simply return the end element of `q1`. Otherwise `q2` is not empty and simply return the end element of `q2`. 

* `push()` Simply add the element to a non-empty queue. If both queues are empty, the new element can be added to an arbitrary queue.

* `pop()` This method removes the newest element. Since all elements are in one of the queues, say it is the `source`, you need to dump all elements except the newest to the other queue. And then remove the last (i.e. the newest) element in the `source`.
  
**Programming Notes:**

Specific tasks, function prototypes, assignment specific components
  
- Prepare your Makefile (you need to construct and add Makefile) so that the TA only needs to invoke the command make to compile your source file and produce the executable file **stack**. Make sure you use exactly the same file names specified here, i.e. **stack**, in your Makefile, otherwise your **submission will get 0 points**.

**Assignment Notes:**

- Include any necessary headers and add necessary global constants.

- You are not allowed to use any I/O functions from the C library, such as scanf or printf. Instead, use the I/O functions from the C++ library, such as cin or cout.

- Add documentation to the appropriate source files as discussed in your class.

When your program is ready for grading, ***commit*** and ***push*** your local repository to remote git classroom repository and follow the _**Assignment Submission Instructions**_.
