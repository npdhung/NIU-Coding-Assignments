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
#include <fstream>

#include "josephus.h"

// reads and initializes all input arguments
void init_vals(std::list<std::string> &list, args &input) {
    ifstream is;

    is.open(FILE);
    if (is.fail()) {
        std::cerr << "Input file opening failed." << std::endl;
        exit(1);
    }
    is >> input.N >> input.M >> input.K;

    // print out input file
    std::cout << "Number of people? " << input.N << std::endl;
    std::cout << "Index for elimination? " << input.M << std::endl;
    std::cout << "Index for printing? " << input.K << std::endl;
    
    // resize the list to the number of people
    size_t s = input.N;
    list.resize(s);
    
    std::generate(list.begin(), list.end(),SEQ(0));
}

// prints all name tags for remaining people after elimination
void print_list ( const std::list <std::string> &list, const unsigned &count ) {
    
    std::cout << std::endl;
    if (count == 0) {
        std::cout << "Initial group of people" << std::endl;
        std::cout << "-----------------------" << std::endl;
    }
    else {
        std::cout << "After eliminating " << count << "th person" << std::endl;
        std::cout << (count>9? "-":"");
        std::cout << "------------------" <<         "----------" << std::endl;
    }
    // print out people list
    int j = 0;
    for (auto &i : list) {
        std::cout << i << " ";
        ++j;
        if (j%NO_ITEMS==0) std::cout << std::endl;
    }
    if (j%NO_ITEMS!=0) std::cout << std::endl;
}

int main() {
    args in;
    std::list <std::string> lst;

    init_vals(lst,in);
    
    print_list(lst,0);

    // eliminates the Mth person
    auto it = lst.begin();
    int skip = in.M;
    int eCount = 0; // count the number of eliminatings
    do {
        // skips to the Mth person
        //std::advance(it, skip-1);
        for (int i=1; i<skip; ++i)
        if (++it == lst.end())
            it = lst.begin();
            // wrap back around to the beginning when falling off the end
        
        // eliminate the person
        auto tempPtr = it;
        if (++it == lst.end())
            it = lst.begin();
            // wrap back around to the beginning when falling off the end
        lst.erase(tempPtr);
        
        ++eCount;
        if (eCount%in.K==0) print_list(lst, eCount);
    } while (lst.size() > 1);
    
    print_list(lst, eCount);
    
    return 0;
}

