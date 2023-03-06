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
#include "twosort.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cstdlib> // c style <stdlib.h>

using std::cout;
using std::endl;
using std::setw;
using std::vector;

void generateVectors(vector<int> &v1, vector<int> &v2, int s1, int s2) {
    srand(s1);
    for (size_t i = 0; i < v1.size();i++)
        v1[i] = rand()%(HIGH - LOW + 1) + LOW;
    
    srand(s2);
    for (size_t i = 0; i < v2.size();i++)
        v2[i] = rand()%(HIGH - LOW + 1) + LOW;
}

bool linearSearch(const vector<int> &v, int x) {
    auto it = std::find(v.begin(), v.end(), x);
    return (it != v.end());
}

bool binarySearch(const vector<int> &v, int x) {
    return std::binary_search(v.begin(), v.end(), x);
}

int searchVector(const vector<int> &v1, const vector<int> &v2, bool(*p)(const vector<int> &,int)) {
    int count = 0;
    for (size_t i = 0; i < v2.size();i++) {
        if (p(v1, v2[i])) count++;
    }
    return count;
}

void sortVector(vector<int> &v) {
    std::sort(v.begin(), v.end());
}

void printVector(const vector<int> &v) {
    for (size_t i = 0; i < v.size();i++) {
        if (i != 0 && i%16 == 0) cout << endl;
        cout << " " << setw(4) << v[i];
    }
    if (v.size()>0) cout << endl;
}

void printStat(int totalSucCnt, int vectorSz) {
    cout.setf(std::ios::fixed);
    cout.setf(std::ios::showpoint);
    cout.precision(2);
    cout << "Percent of Successful Searches = " << static_cast<double>(totalSucCnt*100/vectorSz) << "%" << endl;
}

int main() {
    vector <int> v1(ARRAY_SIZE);
    vector <int> v2(TEST_ARRAY_SIZE);

    generateVectors(v1, v2, SEED1, SEED2);
    printVector(v1);
    cout << endl;
    sortVector(v1);
    printVector(v1);
    cout << endl;
    printVector(v2);

    int totalhit;
    totalhit = searchVector(v1, v2, linearSearch);
    cout << "Linear search: ";
    printStat(totalhit, TEST_ARRAY_SIZE);
    totalhit = searchVector(v1, v2, binarySearch);
    cout << "Binary search: ";
    printStat(totalhit, TEST_ARRAY_SIZE);

    return 0;
}
