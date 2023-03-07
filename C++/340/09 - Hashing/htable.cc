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
#include "htable.h"

#define N1 10  // num of decimal digits
#define N2 26  // num of upper-case letters

// Hash function, you may NOT modify this function
int HT::hash(const string &s) {
  int n = (N2 * N1) * (s[0] - 'A') + N1 * (s[1] - 'A') + (s[2] - '0');

  return n % hsize;
}

/**
 * Constructor for hash table
 *
 * @param tableSize size of the hash table
 *
 * *********************************************************************************/
HT::HT(const unsigned & tableSize) {
    hsize = tableSize;
    hTable = vector<list<Entry>> (hsize);
    //pTable = vector<Entry*> (hsize);
}

/**
 * Destructor for hash table
 *
 * *********************************************************************************/
HT::~HT() {
    auto it = hTable.begin();
    while (it != hTable.end())
    {
        auto temp = it;
        ++it;
        temp->clear();
    }
    hTable.clear();
    
    pTable.clear();
}

bool equal(int a, int b)
{
    return (a == b);
}

/**
 * Insert record of the item e
 *
 * This public function inserts the record of the item e:(key, desc, num) in the hash table.
 * If the key already exists in the table, then the function prints an error message;
 * otherwise, it prints the index value of the inserted record in the hash table and it also
 * inserts the address of the record (in the hash table) into the pointer table. The new
 * record is inserted at the beginning of the corresponding list container.
 *
 * @param e the item inserted to hash table
 *
 * *********************************************************************************/
void HT::insert(const Entry &e) {
    int index = hash(e.key);
    string str = e.key;
    auto it = find_if(hTable[index].begin(), hTable[index].end(), [&str](const Entry& etr){return etr.key == str;});
    if (it == hTable[index].end()) // not found
    {
        cout << right << " entry =  " << index << endl;
        
        // insert the new record at the beginning of the corresponding list container
        hTable[index].push_front(e);
        
        // insert the address of the record (in the hash table) into the pointer table
        Entry* tempPtr = &(*hTable[index].begin());
        
        pTable.push_back(tempPtr);
    }
    else // found
    {
        cout << right << " not inserted - duplicate key!!!" << endl;
    }
}

/**
 * Searches the hash table for a record with key.
 *
 * If the search is successful, it prints the information for the record; otherwise,
 * it prints an error message.
 *
 * @param s the searching string key
 *
 * *********************************************************************************/
void HT::search(const string &s)
{
    bool found = false;

    for (auto vecIt = hTable.begin(); vecIt != hTable.end(); ++vecIt)
    {
        auto it = find_if(vecIt->begin(), vecIt->end(), [&s](const Entry& etr){return etr.key == s;});
        if (it != vecIt->end()) // found
        {
            // print out
            found = true;
            cout << " ==> number: " << setw(4) << right << it->num << " - item:" << setw(ITEM_SZ) << left << it->desc.c_str() << endl;
        }
    }
    if (!found) cout << left << " not in table!!" << endl;
    
}

/**
 * Print the hash table.
 *
 * Print the subscript and the contents of all (and only) the active records in the hash table.
 *
 * *********************************************************************************/
void HT::hTable_print()
{
    cout << endl;
    bool printAdj = false;
    
    if (cout.flags() == 32) // if the previous adjustfield is left
        printAdj = true;
    if (cout.flags() == 128) // if the previous adjustfield is right
        printAdj = false;
    for (auto vecIt = hTable.begin(); vecIt != hTable.end(); ++vecIt)
    {
        for (auto it = vecIt->begin(); it != vecIt->end(); ++it)
            if (it != vecIt->end())
            {
                // print record
                if (printAdj == true) cout << left;
                else cout << right;
                cout << setw(4) << hash(it->key) << ":" << setw(5)<< it->key << "  - ";
                cout << setw(5) << it->num << "  -  ";
                cout << setw(ITEM_SZ) << left << it->desc.c_str() ;
                cout << right << endl;
            }
    }
}

bool cmp(Entry* p, Entry* q)
{
    return ( strcmp(p->key.c_str(), q->key.c_str()) < 0 );
}

/**
 * Print the pTable.
 *
 * This public function prints the contents of all (and only) the active records in the hash table.
 * It first sorts the elements of the pointer table, then prints records in alphabetical order.
 *
 * *********************************************************************************/
void HT::pTable_print()
{
    // sort the vector
    sort(pTable.begin(), pTable.end(),cmp);

    // print pTable
    for (auto it = pTable.begin(); it != pTable.end(); ++it)
    {
        cout << "   "  << (*it)->key << "  -  " << setw(4)
            << right << (*it)->num << "  -  " << setw(ITEM_SZ) << left
            << (*it)->desc.c_str() << endl;
    }
}
