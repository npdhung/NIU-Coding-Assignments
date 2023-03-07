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
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#ifndef H_BST
#define H_BST

#include "btree.h"

#define SEED 1      // seed for RNG
#define N 100       // num of rand ints ///XXX remember to change back to 100

#define NO_ITEMS 16 // max num of elems printed in line
#define ITEM_W 3    // size of each elem in print

unsigned sz;        // size of vector/binTree

// macro to print size
#define COUT_SZ cout << "size = " << setw ( ITEM_W ) << sz << " :"

// class to generate rand ints
class RND {
private:
    int seed;   
public:
    RND ( const int& s = SEED ) : seed ( s ) { srand ( seed ); }
    int operator ( ) ( ) { return rand ( ) % N + 1; }
};

// function to print elems on stdout
template < typename T > void print ( const T& x )
{
    static unsigned cnt = 0;
    const string sp ( 12, ' ' );

    cout << setw ( ITEM_W ) << x << ' '; cnt++;
    if ( cnt % NO_ITEMS == 0 || cnt == sz ) cout << endl << sp;
    if ( cnt == sz ) cnt = 0;
}

template <typename T>
class BinarySearchTree : public BinaryTree<T>
{
public:
    /**
     * Insert node with data value val to a tree
     *
     * @param x value of the node
     *
     * *********************************************************************************/
    void Insert(const T &x);
    
    /**
     * Search for a leaf node with data value val
     *
     * @param x value of the node
     *
     * @return true if a leaf node with value val is found in a tree
     *
     * *********************************************************************************/
    bool Search(const T &x) const;
    
    /**
     * Remove a leaf node of a tree
     *
     * @param x value of the node
     *
     * @return true if successfully remove a leaf node
     *
     * *********************************************************************************/
    bool Remove(const T &x);
private:
    
    /**
     * Insert node with data value val to a tree
     *
     * @param ptr point to the root node of the tree
     * @param val value of the node
     *
     * *********************************************************************************/
    void _Insert(Node<T> *& ptr, const T &val);
    
    /**
     * Search for a leaf node with data value val
     *
     * @param node the root node of the tree
     * @param val value of the node
     *
     * @return true if a leaf node with value val is found
     *
     * *********************************************************************************/
    bool _Search(Node<T> * node, const T &val) const;

    void _deleteRoot(Node<T>* node);            ///< helper function for _Remove
    Node<T>* _deleteLeaves(Node<T>* &root, T x);///< helper function for _Remove
    
    /**
     * Remove a leaf node of a tree
     *
     * @param ptr the root node of the tree
     * @param val value of the node
     *
     * *********************************************************************************/
    void _Remove(Node<T> *& ptr, const T &val); ///< private version of remove
    bool _Leaf(Node<T> *node) const;            ///< checks if node is leaf
};

#endif
