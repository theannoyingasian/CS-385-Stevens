/*******************************************************************************
 * Name        : shortestpaths.cpp
 * Author      : Himmat Garcha, Stephen D'Agostino
 * Version     : 1.0
 * Date        : 12/4/24
 * Description : Implementation of red-black tree.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System
 ******************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

const long MAX_INT = numeric_limits<long>::max();

int num_vertices;

void split(vector<string>* result, const string& str, char delimiter) { 
    stringstream ss(str); 
    string item; 
    while (getline(ss, item, delimiter)) { 
        result->push_back(item);
    }
}

bool checkConditions(string line, int lineNumber) {
    int temp;
    if (lineNumber == 1) {
        try {
            temp = stoi(line);
        }
        catch (const std::invalid_argument &e) {
            cerr << "Error: Invalid number of vertices '" << line << "' on line 1." << endl;
            return false;
        }
        if (temp <= 0 || temp > 26) {
            cerr << "Error: Invalid number of vertices '" << line << "' on line 1." << endl;
            return false;
        }
        return true;
    }

    vector<string> array;
    split(&array, line, ' ');
    if (array.size() != 3) {
        cerr << "Error: Invalid edge data '" << line << "' on line " << lineNumber << "." << endl;
        return false;
    }

    if (array.at(0).length() != 1 || int(array.at(0).at(0)) - 64 > num_vertices || int(array.at(0).at(0)) - 64 <= 0) {
        cerr << "Error: Starting vertex '" << array.at(0) << "' on line " << lineNumber << " is not among valid values A-" << char(num_vertices + 64) << "." << endl;
        return false;
    }

    if (array.at(1).length() != 1 || int(array.at(1).at(0)) - 64 > num_vertices || int(array.at(1).at(0)) - 64 <= 0) {
        cerr << "Error: Ending vertex '" << array.at(1) << "' on line " << lineNumber << " is not among valid values A-" << char(num_vertices + 64) << "." << endl;
        return false;
    }

    try {
        temp = stoi(array.at(2));
    }
    catch (const std::invalid_argument &e) {
        cerr << "Error: Invalid edge weight '" << array.at(2) << "' on line " << lineNumber << "." << endl;
        return false;
    }
    if (temp <= 0) {
        cerr << "Error: Invalid edge weight '" << array.at(2) << "' on line " << lineNumber << "." << endl;
        return false;
    }
    return true;
}

void display_table(const vector<vector<long>>& matrix, const string &label, const bool use_letters = false) {
    cout << label << endl;
    long max_val = 0;
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            long cell = matrix[i][j];
            if (cell < MAX_INT && cell > max_val) {
                max_val = matrix[i][j];
            }
        }
    }
    int max_cell_width = use_letters ? to_string(max_val).length() :
                        to_string(max(static_cast<long>(num_vertices), max_val)).length();
    cout << ' ';
    for (int j = 0; j < num_vertices; j++) {
        cout << setw(max_cell_width + 1) << static_cast<char>(j + 'A');
    }
    cout << endl;
    for (int i = 0; i < num_vertices; i++) {
        cout << static_cast<char>(i + 'A');
        for (int j = 0; j < num_vertices; j++) {
            cout << " " << setw(max_cell_width);
            if (matrix[i][j] == MAX_INT) {
                cout << "-";
            } else if (use_letters) {
                cout << ((matrix[i][j] >= 0) ? static_cast<char>(matrix[i][j] + 'A') : '-');
            } else {
                cout << matrix[i][j];
            }
        }
        cout << endl;
    }
    cout << endl;
}

void floyds_int_vertices(vector<vector<long>> &dist, vector<vector<long>> &next) {
    for (int k = 0; k < num_vertices; k++) {
        for (int i = 0; i < num_vertices; i++) {
            for (int j = 0; j < num_vertices; j++) {
                if (dist[i][k] != MAX_INT && dist[k][j] != MAX_INT && dist[i][j] > dist[i][k] + dist[k][j]) {
                    if(dist[i][k] + dist[k][j] >= 0){
                        dist[i][j] = dist[i][k] + dist[k][j];
                        next[i][j] = k; // Store the intermediate vertex
                    }
                }
            }
        }
    }
}

void reconstruct_path(const vector<vector<long>>& next, long u, long v, vector<long>& path) {
    int intermediate = next[u][v];
    
    if (intermediate == -1) return;
    if (intermediate == u || intermediate == v) return;  // Prevent self-loop or redundant calls

    reconstruct_path(next, u, intermediate, path);
    path.push_back(intermediate);
    reconstruct_path(next, intermediate, v, path);
}

void printPaths(const vector<vector<long>>& dist, const vector<vector<long>>& next) {
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            if (i != j) {
                if (dist[i][j] == MAX_INT) {
                    cout << static_cast<char>(i + 'A') << " -> " << static_cast<char>(j + 'A') << ", distance: infinity, path: none" << endl;
                } else {
                    cout << static_cast<char>(i + 'A') << " -> " << static_cast<char>(j + 'A') << ", distance: " << dist[i][j] << ", path: ";
                    vector<long> path;
                    path.push_back(i);
                    reconstruct_path(next, i, j, path);
                    path.push_back(j);
                    for (size_t k = 0; k < path.size(); ++k) {
                        cout << static_cast<char>(path[k] + 'A');
                        if (k < path.size() - 1) {
                            cout << " -> ";
                        }
                    }
                    cout << endl;
                }
            } else {
                cout << static_cast<char>(i + 'A') << " -> " << static_cast<char>(j + 'A') << ", distance: " << dist[i][j] << ", path: " << static_cast<char>(i + 'A') << endl;
            }
        }
    }
}

vector<vector<long>> makeTable(const vector<vector<long>>& order) { 
    vector<vector<long>> table(num_vertices, vector<long>(num_vertices, MAX_INT)); 
    for (int i = 0; i < num_vertices; i++) { 
        table[i][i] = 0; 
    } 
    for (const auto& row : order) { 
        if (row.size() == 3) { 
            table[row[0]][row[1]] = row[2]; 
        } 
    } return table;
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    ifstream input_file(argv[1]);
    if (!input_file) {
        cerr << "Error: Cannot open file '" << argv[1] << "'." << endl;
        return 1;
    }
    input_file.exceptions(ifstream::badbit);
    string line;
    vector<vector<long>> order;

    try {
        unsigned int line_number = 1;
        while (getline(input_file, line)) {
            if (!checkConditions(line, line_number)) {
                input_file.close();
                return 1;
            }

            if (line_number == 1) {
                num_vertices = stoi(line);
            } else {
                vector<string> array;
                split(&array, line, ' ');

                vector<long> list;
                for (size_t i = 0; i < array.size(); ++i) {
                    if (i < array.size() - 1) {
                        list.push_back(array[i][0] - 'A');  // Assuming vertices are letters A-Z
                    } else {
                        list.push_back(stoi(array[i]));
                    }
                }
                order.push_back(list);
            }
            ++line_number;
        }
        input_file.close();
    } catch (const ifstream::failure &f) {
        cerr << "Error: An I/O error occurred reading '" << argv[1] << "'." << endl;
        return 1;
    }

    vector<vector<long>> table = makeTable(order);
    vector<vector<long>> next(num_vertices, vector<long>(num_vertices, -1));

    for(int k = 0; k < num_vertices; k++){
        for (int i = 0; i < num_vertices; i++) {
            for (int j = 0; j < num_vertices; j++) {
                if (table[i][j] != MAX_INT && i != j) {
                    if (table[i][k] != MAX_INT && table[k][j] != MAX_INT &&
                        table[i][j] > table[i][k] + table[k][j]) {
                        if(table[i][k] + table[k][j] >= 0){
                            next[i][j] = next[i][k]; // Update next to go through vertex k
                        }
                    }
                }
            }
        }
    }

    display_table(table, "Distance matrix:");
    floyds_int_vertices(table, next);
    display_table(table, "Path lengths:");
    display_table(next, "Intermediate vertices:", true);
    printPaths(table, next);

    return 0;
}
