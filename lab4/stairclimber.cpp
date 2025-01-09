/*******************************************************************************
 * Name        : stairclimber.cpp
 * Author      : Himmat Garcha
 * Date        : 10/3/24
 * Description : Lists the number of ways to climb n stairs.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

vector< vector<int> > get_ways(int num_stairs) {
    // TODO: Return a vector of vectors of ints representing
    // the different combinations of ways to climb num_stairs
    // stairs, moving up either 1, 2, or 3 stairs at a time.
    if (num_stairs <= 0){
        return {{}};
    }
    vector <vector<int>> ways;
    for (int i = 1; i <= 3; i++){
        if (num_stairs  >= i){
            vector <vector<int>> result = get_ways(num_stairs - i);
            for (auto &way : result){
                way.insert(way.begin(), i);
            }
            ways.insert(ways.end(), result.begin(), result.end());
        }
    }
    return ways;
}

void display_ways(const vector< vector<int> > &ways) {
    // TODO: Display the ways to climb stairs by iterating over
    // the vector of vectors and printing each combination.
    int width = to_string(ways.size()).length();
    for (size_t i = 0; i < ways.size(); i++){
        cout << setw(width) << (i + 1) << ". [";
        for (size_t j = 0; j < ways[i].size(); j++){
            cout << ways[i][j];
            if (j < ways[i].size() - 1){
                cout << ", ";
            }
        }
        cout << "]" << endl;
    }
}

int main(int argc, char * const argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <number of stairs>" << endl;
        return 1;
    }
    istringstream iss(argv[1]);
    int num_stairs;
    if (!(iss >> num_stairs) || num_stairs <= 0) {
        cerr << "Error: Number of stairs must be a positive integer." << endl;
        return 1;
    }
    vector<vector<int>> ways = get_ways(num_stairs);
    cout << ways.size() << " way" << (ways.size() == 1 ? "" : "s") << " to climb " << num_stairs << " stair" << (num_stairs == 1 ? "" : "s") << "." << endl;
    display_ways(ways);
    return 0;
}