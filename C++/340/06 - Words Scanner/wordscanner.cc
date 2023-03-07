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
#include <string>
#include <map>
#include <cstring>
#include <iomanip>
#include <algorithm>

#define NO_ITEMS 4
#define ITEM_WORD_WIDTH 14
#define ITEM_COUNT_WIDTH 3

using std::cout;
using std::endl;

size_t iCount = 0;

/**
 * Cleans a word from its punctuation marks.
 *
 * @param inStr input word
 * @param subStr word after removing its punctuation marks
 *
 * *********************************************************************************/
void clean_entry(const std::string& inStr, std::string& subStr) {
            
    char charArray[50];
    size_t start = 0;
    size_t end = 0;
    strcpy(charArray, inStr.c_str());

    if (!isalnum(charArray[0]))
    {
        // Remove pair of matching punctuation
        // or the beginning punctuation if there is no pair of punctuation
        start = 1;
        end = inStr.size();
        for (size_t i = 1; i < inStr.size(); ++i)
            if (!isalnum(charArray[i])) {
                end = i; //> position of the matching punctuation
                break;
            }
    } else {   // Remove the ending punctuation(s)
        start = 0;
        end = inStr.size();
        for (size_t i = 1; i < inStr.size(); ++i)
            if (!isalnum(charArray[i])) {
                end = i; //> position of the ending punctuation or end of line
                break;
            }
    }
    subStr = inStr.substr(start, end-start);
    
    // Convert to lower case
    std::for_each(subStr.begin(), subStr.end(), [](char & c){
        c = std::tolower(c);});
}

/**
 * Gets a word from the input stream and removes its punctuation marks
 *
 * @param wordsMap a map containers of all words
 *
 * *********************************************************************************/
void get_words(std::map<std::string, int>& wordsMap) {
    std::string input;
    
    std::cin >> input;
    while (!std::cin.eof()) {

        // Clean all string that contains only punctuation marks
        bool word = false; //> return false if the string contains only punctuation marks
        char charArray[50];
        strcpy(charArray, input.c_str());
        for (int i = 0; charArray[i] != '\0'; ++i) {
            if (isalnum(charArray[i])) word = true;
        }
        if (word) {
            ++iCount;
            std::string subStr;
            clean_entry(input, subStr);
            auto r = wordsMap.insert({subStr,1});
            
            // Increase the frequency if seeing the same word
            if (r.second == 0) {
                ++(wordsMap.at(subStr));
            }
        }
            std::cin >> input;
    }
}

/**
 * prints the final list of words and their frequencies, also prints the number of nonempty
 * words and the number of distinct words in the input stream
 *
 * @param wordsMap a map containers of all words
 *
 * *********************************************************************************/
void print_words(const std::map<std::string, int>& wordsMap) {
    int colCount = 0;
    
    for (auto i = wordsMap.begin(); i != wordsMap.end(); ++i) {
        ++colCount;

        cout << std::left << std::setw(ITEM_WORD_WIDTH) << i->first << ":" << std::setw(ITEM_COUNT_WIDTH) << i->second;
        if (colCount % NO_ITEMS == 0) cout << endl;
    }
    if (colCount % NO_ITEMS != 0) cout << endl;
    cout << endl;
    cout << "no of words in input stream   :" << iCount << endl;
    cout << "no of words in output stream  :" << wordsMap.size() << endl;
}

int main() {
    std::map<std::string, int> wordsMap;
    
    get_words(wordsMap);
    print_words(wordsMap);

    return 0;
}
