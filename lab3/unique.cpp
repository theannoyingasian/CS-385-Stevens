/*******************************************************************************
 * Name        : unique.cpp
 * Author      : Himmat Garcha
 * Date        : 9/28/24
 * Description : Determining uniqueness of chars with int as bit vector.
 * Pledge      : I pledge my honor that I have abided by the stevens honor system
 ******************************************************************************/
#include <iostream>
#include <cctype>
#include <cstring>
using namespace std;

/*
checks if characters in the string are lower case
if true returns true if not returns false
 */
bool is_all_lowercase(const string &s) {
    for (char c : s) {
        if (!islower(c)) {
            return false;
        }
    }
    return true;
}

/*
checks if the letters in string are unique by using bitwise operations
returns true if letters are unique if not returns false
*/
bool all_unique_letters(const string &s) {
    unsigned int checker = 0;
    for (char c : s) {
        int bit_index = c - 'a';
        if (checker & (1 << bit_index)) {
            return false;
        }
        checker |= (1 << bit_index);
    }
    return true;
}

/*
main fn 
checks input 
if program runs returns 0 otherwise returns 1
*/
int main(int argc, char * const argv[]) {
    if (argc != 2) {
        cout << "Usage: ./unique <string>" << endl;
        return 1;
    }

    string input = argv[1];

    if (!is_all_lowercase(input)) {
        cout << "Error: String must contain only lowercase letters." << endl;
        return 1;
    }

    if (all_unique_letters(input)) {
        cout << "All letters are unique." << endl;
    } else {
        cout << "Duplicate letters found." << endl;
    }

    return 0;
}
