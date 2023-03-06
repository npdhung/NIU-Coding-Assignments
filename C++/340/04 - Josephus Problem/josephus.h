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
#ifndef JOSEPHUS_H
#define JOSEPHUS_H

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <algorithm>

using namespace std;

#define NO_LETS  26         ///> no of letters in English alphabet
#define NO_ITEMS 12         ///> no of items printed on single line
#define FILE "josephus.d"   ///> input filename

/**
 * Struct for input arguments.
 */
struct args {
    unsigned N,       /**< no of initial people */
             M,       /**< count to eliminate person. */
             K;       /**< frequency of printouts. */
};

/**
 * Class to generate name tags for people.
 */
class SEQ
{
private:
    string id;          /**< name tag for person. */
    unsigned size;      /**< no of people. */
    unsigned nd;        /**< no of digits in name tags. */

    /**
     * Returns no of digits in name tags.
     * @param sz size of the list.
     */
    unsigned find_nd ( const double& sz ) {
        if ( ( sz / NO_LETS ) <= 1 ) return 2;
        else return ( find_nd ( sz / NO_LETS ) + 1 );
    }

public:
    /**
     * Constructor for name-tag generator
     */
    SEQ ( const unsigned& s = 1 ) : size ( s ) {
        double sz = ( double ) size / 9;
        nd = find_nd ( sz );
        id = string ( nd, 'A' );
        id [ nd - 1 ] = '1';
    }

    /**
     * Returns next name tag in sequence
     */
    string operator ( ) ( ) {
        string tmp = id; int i = nd - 1;
        if ( id [ i ] < '9' ) id [ i ]++;
        else {
            id [ i ] = '1'; bool flag = true;
            for ( i--; i >= 0 && flag; i-- )
                if ( id [ i ] < 'Z' ) { id [ i ]++; flag = false; }
                else id [ i ] = 'A';
        } 
        return tmp;
    }
};

/**
 * Reads and initializes all input arguments.
 * @param list the list of people names.
 * @param input input data.
 */
void init_vals(list<string> &list, args &input);

/**
 * Prints all name tags for remaining people after elimination
 * @param list the list of people.
 * @param count the elimination count.
 */
void print_list ( const list < string > &list, const unsigned &count);

#endif
