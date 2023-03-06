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
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

#ifndef H_MATRIX_MULTIPLY
#define H_MATRIX_MULTIPLY

// data files
constexpr char FILE1[] = "./matrix01.dat";
constexpr char FILE2[] = "./matrix02.dat";

#define ITEM_W 8 // no of spaces for each printed number

/**
 * Opens two data files.
 *
 * Opens two data files for reading and creates the input streams is1 and is2.
 * If either file fails to open, it prints an error message on stderr and aborts the
 * program with a non-zero exit.
 *
 * @param is1 First input stream.
 * @param is2 Second input stream.
 *
 *********************************************************************************/
void open_files(ifstream &is1, ifstream &is2);

/**
 * Reads input values.
 *
 * Reads input value from the input stream and store them in the matrix m,
 * which is defined as a two-dimensional vector of integers. The number of
 * rows and columns of m are the pair of values given at the beginning of
 * the corresponding data file.
 *
 * @param is Input stream.
 * @param m The matrix that stores input data.
 *
 *********************************************************************************/
void read_data(ifstream &is, vector <vector <int>> &m);

/**
 * Prints the contents of the matrix..
 *
 * Prints the contents of the matrix on stdout by allocating ITEM_W spaces
 * for each integer value, which is defined in the header.
 *
 * @param m The matrix that stores input data.
 *
 *********************************************************************************/
void print_data(const vector <vector<int>> &m);

/**
 * Generate the matrix by multiplying two matrices.
 *
 * Generate the matrix C by multiplying two matrices A and B.
 *
 * @param A First matrix.
 * @param B Second matrix.
 * @param C The resulting matrix.
 *
 *********************************************************************************/
void gen_data(const vector <vector<int>> &A,
    			const vector <vector<int>> &B,
    			vector <vector<int>> &C);

#endif
