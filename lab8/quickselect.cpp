/*******************************************************************************
 * Name          : quickselect.cpp
 * Author        : Himmat Garcha
 * Pledge        : I pledge my honor that I have abided by the Stevens Honor System
 * Date          : 10/25/24
 * Description   : Implements the quickselect algorithm.
 ******************************************************************************/
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

size_t lomuto_partition(int array[], size_t left, size_t right) {
    size_t mid = left + (right - left) / 2;
    if (array[mid] < array[left]){ 
        swap(array[left], array[mid]);
    }
    if (array[right] < array[left]){
        swap(array[left], array[right]);
    }
    if (array[right] < array[mid]){
        swap(array[mid], array[right]);
    }
    int p = array[mid];
    swap(array[mid], array[left]);
    size_t s = left;
    for (size_t i = left + 1; i <= right; i++) {
        if (array[i] < p) {
            s++;
            swap(array[s], array[i]);
        }
    }
    swap(array[left], array[s]);
    return s;
}

int quick_select(int array[], size_t left, size_t right, size_t k) {
    while (true) {
        size_t s = lomuto_partition(array, left, right);
        if (s == k - 1) {
            return array[s];
        } else if (s > k - 1) {
            right = s - 1;
        } else {
            left = s + 1;
        }
    }
}

int quick_select(int array[], const size_t length, size_t k) {
    return quick_select(array, 0, length - 1, k);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <k>" << endl;
        return 1;
    }

    int k;
    istringstream iss;
    iss.str(argv[1]);
    if ( !(iss >> k) || k <= 0 ) {
        cerr << "Error: Invalid value '" << argv[1] << "' for k." << endl;
        return 1;
    }

    cout << "Enter sequence of integers, each followed by a space: " << flush;
    int value, index = 0;
    vector<int> values;
    string str;
    str.reserve(11);
    char c;
    iss.clear();
    while (true) {
        c = getchar();
        const bool eoln = c == '\r' || c == '\n';
        if (isspace(c) || eoln) {
            if (str.length() > 0) {
                iss.str(str);
                if (iss >> value) {
                    values.push_back(value);
                } else {
                    cerr << "Error: Non-integer value '" << str
                         << "' received at index " << index << "." << endl;
                    return 1;
                }
                iss.clear();
                ++index;
            }
            if (eoln) {
                break;
            }
            str.clear();
        } else {
            str += c;
        }
    }

    int num_values = values.size();
    if (num_values == 0) {
        cerr << "Error: Sequence of integers not received." << endl;
        return 1;
    }
    if(k > num_values){
        cerr << "Error: Cannot find smallest element " << k << " with only " << num_values;
        if (num_values == 1){
            cerr << " value." << endl;
        }
        else{
            cerr << " values." << endl;
        }
        return 1;
    }
    cout << "Smallest element " << k << ": " << quick_select(values.data(), num_values, k) << endl;
    return 0;
}
