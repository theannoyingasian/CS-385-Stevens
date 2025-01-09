/*******************************************************************************
 * Name        : inversioncounter.cpp
 * Author      : Himmat Garcha
 * Version     : 1.0
 * Date        : 11/2/2024
 * Description : Counts the number of inversions in an array.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System
 ******************************************************************************/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;

static long mergesort(int array[], int scratch[], int low, int high);

// counts num of inversions in array in time theta n^2 
long count_inversions_slow(int array[], int length) {
    long count = 0;
    for(int i = 0; i < length - 1; i++){
        for(int j = i + 1; j < length; j++){
            if(array[i] > array[j]){
                count++;
            }
        }
    }
    return count;
}

// counts num of inversions in array in time theta n lg n
long count_inversions_fast(int array[], int length) {
    int* arr = new int[length];
    long count = mergesort(array, arr, 0, length - 1);
    delete[] arr;
    return count;
}

static long merge(int a[], int b[], int low, int mid, int high){
    long count = 0;
    int left = low, right = mid+1, i = low;
    while(left <= mid && right <= high){
        if(a[left] <= a[right]){
            b[i++] = a[left++];
        }else{
            b[i++] = a[right++];
            count += (mid + 1 - left);
        }
    }
    while(left <= mid){
        b[i++] = a[left++];
    }
    while(right <= high){
        b[i++] = a[right++];
    }
    for(int n1 = low; n1 <= high; n1++){
        a[n1] = b[n1];
    }
    return count;
}

static long mergesort(int array[], int scratch[], int low, int high) {
    long count = 0;
    if(low < high){
        int mid = low + (high - low)/2;
        count += mergesort(array, scratch, low, mid);
        count += mergesort(array, scratch, mid + 1, high);
        count += merge(array, scratch, low, mid, high);
    }
    return count;

}

int main(int argc, char *argv[]) {
    if(argc != 1 && argc != 2){
        cerr << "Usage: ./inversioncounter [slow]" << endl;
        return 1;
    }
    if(argc == 2){
        string compare = argv[1];
        if(compare != "slow"){
            cerr << "Error: Unrecognized option '" << compare << "'." << endl;
            return 1;
        }
    }

    cout << "Enter sequence of integers, each followed by a space: " << flush;

    istringstream iss;
    int value, index = 0;
    vector<int> values;
    string str;
    str.reserve(11);
    char c;
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

    if(values.empty()){
        cerr << "Error: Sequence of integers not received." << endl;
        return 1;
    }

    if(argc == 2){
        cout << "Number of inversions (slow): " << count_inversions_slow(&values[0], values.size()) << endl;
        return 0;
    }

    cout << "Number of inversions (fast): " << count_inversions_fast(&values[0], values.size()) << endl;
    return 0;
}
