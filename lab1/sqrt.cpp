/*******************************************************************************
* Filename: sqrt.cpp
* Author : Himmat Garcha
* Version : 1.0
* Date : 9/10/2024
* Description: Square root program, using newtons method this program when ran will 
*               reuturn the last/smallest value of the orginal number in 8 bit
* Pledge : I pledge my honor that I have abided by the Stevens Honor System
******************************************************************************/
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
using namespace std;

bool isDouble(const string& str, double& value) {
   // standalone method to check if the numbers and epsilons are doubles
    istringstream iss(str);
    iss >> value;
    return !iss.fail() && iss.eof();
}

bool isPositiveDouble(const string& str, double& value) {
     /* used to verify if all numbers and epsilons plugged are indeed postive AND doubles
     * this was done as for some reason when ran some would still slip through so made 
     * a seperate method to make sure none would get past 
     */
    return isDouble(str, value) && value > 0;
}

double sqrt(double num, double epsilon) {

    // if the number passed used to check if the number less than zero
    if (num < 0.0) {
        return NAN;
    }

    // if the number is = to 0 or 1 then it is already as small as it can get so just returns it
    if (num == 0.0 || num == 1.0) {
        return num;
    }

    // if passed previous 2 conditions then the number is set as the previouys guess
    double last_guess = num;
    // the next guess/current guess is the square root of the last guess
    double next_guess = (last_guess + num / last_guess) / 2;

    // to make sure that the diffrenece between the 2 guess is with in the margins defined by epsilon
    while (abs(last_guess - next_guess) > epsilon) {
        last_guess = next_guess;
        next_guess = (last_guess + num / last_guess) / 2;
    }

    return next_guess;
}

int main(int argc, char* argv[]) {
    
    // check char length of number and epslion entered if incorrect length returns a error
    
    if (argc < 2 || argc > 3) {
        cerr << "Usage: " << argv[0] << " <value> [epsilon]" << endl;
        return 1;
    }

    //implementation of the isDouble method to check the nums to make sure they pass
    double num;
    if (!isDouble(argv[1], num)) {
        cerr << "Error: Value argument must be a double." << endl;
        return 1;
    }
    //implementation of the isPostiveDouble to check to see if the epsilons pass

    double epsilon = 1e-7; // Default value for epsilon
    if (argc == 3) {
        if (!isPositiveDouble(argv[2], epsilon)) {
            cerr << "Error: Epsilon argument must be a positive double." << endl;
            return 1;
        }
    }

    // if num and epsilon pass runs the sqrt fn
    double value = sqrt(num, epsilon);

    // convert the value of the sqrt fn to 8bit
    cout << fixed << setprecision(8) << value << endl;

    return 0;
   
}

