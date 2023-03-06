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
#include "matrix.h"

/*
   This program opens two data files for reading. If either file can't
   be opened, it prints an error message on stderr and aborts the
   program. It generates two input matrices from the data in data
   files, then generates a new matrix by multiplying the numbers in
   two input matrices. Finally, it prints all three matrices on
   stdout.
*/

// to open two data files
void open_files(ifstream &is1, ifstream &is2) {
    is1.open(FILE1);
    is2.open(FILE2);
    if (is1.fail()||(is2.fail())) {
        std::cerr << "Input file opening failed." << endl;
        exit(1);
    }
}

// to read contents of two data files
void read_data(ifstream &is, vector <vector <int>> &m) {
    for (size_t i = 0; i < m.size(); i++)
        for (size_t j = 0; j < m[0].size(); j++) is >> m[i][j];
}

// to print contents of all matrices
void print_data(const vector <vector<int>> &m) {
    std::cout << m.size() << " x " << m[0].size() << endl;
    for (int i = 1; i <= 70; i++) std::cout <<"-";
    cout << endl;
    for (auto i : m) {
        for (auto j : i) std::cout << setw(ITEM_W) << j;
        cout << endl;
    }
}

// to generate a matrix by multiplying two input matrices
void gen_data(const vector <vector<int>> &A,
                const vector <vector<int>> &B,
              vector <vector<int>> &C) {
    for (size_t i = 0; i < C.size(); i++)
        for (size_t j = 0; j < C[0].size(); j++) {
            C[i][j] = 0;
            for (size_t k = 0; k < A[0].size(); k++) C[i][j] += A[i][k] * B[k][j];
        }
}

int main ( )
{
    ifstream is1, is2;      // input file streams
    open_files(is1, is2);   // open input files

    // number of rows and colums in matrices A, B and C
    unsigned nrowsA, ncolsA, nrowsB, ncolsB, nrowsC, ncolsC;

    // read number of rows and colums for input matrices from data
    // files and assign number of rows and colums to resulting
    // matrix from values of input matrices; first two values in
    // data file are number rows and colums of a matrix
    is1 >> nrowsA >> ncolsA; 
    is2 >> nrowsB >> ncolsB;
    nrowsC = nrowsA; 
    ncolsC = ncolsB;

    // define matrices as two-dimensional vectors
    vector<vector<int>> A(nrowsA, vector<int>(ncolsA));
    vector<vector<int>> B(nrowsB, vector<int>(ncolsB));
    vector<vector<int>> C(nrowsC, vector<int>(ncolsC));
    
    // read input values from data files
    read_data(is1, A); 
    read_data(is2, B);

    // generate values for matrix C by multiplying values in
    // matrices A and B, where C = AB
    gen_data(A, B, C);

    // print contents of all matrices on stdout
    cout << "\nMatrix1: "; print_data(A);
    cout << "\nMatrix2: "; print_data(B);
    cout << "\nMatrix3: "; print_data(C);
    cout << endl;

    return 0;
}
