### CSCI 340 - Data Structures and Algorithm Analysis

Data Structure: Vector 
Programming Focus: Use of STL vector and review of C++

# Matrix Multiplication

For this computer assignment, you are to write and implement a C++ program to implement the matrix multiplication on randomly generated integers stored in data files.

Put the implementations of your subroutines (described below) in your source file **matrix_sub.cc**. **NOTE:** You will need to create this file, you will need to do a `git add matrix_sub.cc`, once you create it in your local repository and it will then be added to your GitHub repostority after you do a `git commit` and `git push`. **NOTE:** The repository as is will not compile to an executable at the start due to missing functions.

- `void open_files(ifstream &is1, ifstream &is2)`: Opens two data files for reading and creates the input streams `is1` and `is2`. If either file fails to open, it prints an error message on stderr and aborts the program with a non-zero exit value. The names of the input files are `matrix01.dat` and `matrix02.dat`, whose full path names are defined as `FILE1` and `FILE2` in the header file `matrix.h` all included within starter repository.

- `void read_data(ifstream &is, vector<vector<int>> &m)`: Reads the input values from the input stream is and stores them in matrix `m`, which is defined as a two-dimensional vector of integers. The number of rows and columns of `m` are the pair of values given at the beginning of the corresponding data file and they are captured in the source file `matrix.cc` (driver program) that is in the same directory with `matrix.h`.

- `void print_data(const vector<vector<int>> &m)`: Prints the contents of on stdout by allocating `ITEM_W` spaces for each integer value, which is defined in the header file `matrix.h`. The number of values printed in a single line equals to the number of columns of `m` and the number of lines on stdout equals to the number of rows of `m`.

- `void gen_data(const vector<vector<int>> &A, const vector<vector<int>> &B, vector<vector<int>> &C)`: It multiples the values in the matrices `A` and `B` and stores the results in the matrix `C`. 
    - If `C(i, j)` is the value of `C` in row `i` and column `j` of `C`, then `C(i, j) += A(i, k) * B(k, j)` for all index values of `k`, where `A(i, k)` is the value of `A` in row `i` and column `k` of `A`, and `B(k, j)` is the value of `B` in row `k` and column `j` of `B`. 
    - For a vector container use `C[i][j]` for `C(i, j)`, where `i = 0, 1, ..., size(C) - 1` and `j = 0, 1, ..., C[i].size() - 1`, and it's the same for `A` and `B`.
    - See also: (https://en.wikipedia.org/wiki/Matrix_multiplication)

**Programming Notes:**

- Do not change existing implementation in `matrix.cc`, but you need to include any necessary headers and add necessary global constants.

- The header file matrix.h is provided. To include the header file in your program, insert the statement: `#include "matrix.h"` at the top of your source file `matrix_sub.cc`. 

- Correct output is found in reference.out based the matrix values in data files matrix01.dat and matrix02.dat, these are just example. They can be changed, so do not design your program for the specific values in the data files. Your program is supposed to work for any matrix values in those files. 

- You are not allowed to use any I/O functions from the C library, such as `scanf` or `printf`. Instead, use the I/O functions from the C++ library, such as `cin` or `cout`.

- Add documentation to the appropriate your source files as discussed in lecture.

- We have included a `Makefile` for your reference, in future assignments you will need to build your own. To compile your code just type `make` in the project directory. To test your program, execute `./matrix &> matrix.out`, which will put the output (including any error messages) in file `matrix.out`. You can find the correct output of this program in file `matrix.refout`, you can compare the two files by using `diff` on turing or hopper, `diff matrix.out matrix.refout`, if the files are the same there will be no output from `diff`.

When your program is ready for grading, ***commit*** and ***push*** your local repository to remote git classroom repository and follow the _**Assignment Submission Instructions**_.
