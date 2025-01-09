/*******************************************************************************
 * Name        : sieve.cpp
 * Author      : Himmat Garcha
 * Date        : 9/18/2024
 * Description : Sieve of Eratosthenes
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System
 ******************************************************************************/
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class PrimesSieve {
public:
    PrimesSieve(int limit);

    ~PrimesSieve() {
        delete [] is_prime_;
    }

    void display_primes() const;

private:
    // Instance variables
    bool * const is_prime_;
    const int limit_;
    int num_primes_, max_prime_;

    // Method declarations
    void sieve();
    static int num_digits(int num);
};

PrimesSieve::PrimesSieve(int limit) :
        is_prime_{new bool[limit + 1]}, limit_{limit} {
    sieve();
}

void PrimesSieve::display_primes() const {
    const int max_prime_width = num_digits(max_prime_), primes_per_row = 80 / (max_prime_width + 1);
    int num_count = 0;
    cout << endl << "Number of primes found: " << num_primes_ << endl;
    cout << "Primes up to " << limit_ << ":" << endl;
    for (int i = 2; i <= limit_; i++) {
        if (is_prime_[i]) {
            num_count++;
            if (num_primes_ > primes_per_row) {
                cout << setw(max_prime_width);
            }
            cout << i;
            if (num_count % primes_per_row == 0) {
                cout << endl;
                num_count = 0;
            }else if (i != max_prime_) {
                cout << " ";
            }
        }
    }
    cout << endl;
}

void PrimesSieve::sieve() {
    fill(is_prime_, is_prime_ + limit_ + 1, true);
    is_prime_[0] = is_prime_[1] = false;

    for (int i = 2; i <= sqrt(limit_); ++i) {
        if (is_prime_[i]) {
            for (int j = i * i; j <= limit_; j += i) {
                is_prime_[j] = false;
            }
        }
    }

    num_primes_ = 0;
    max_prime_ = 2;
    for (int i = 2; i <= limit_; ++i) {
        if (is_prime_[i]) {
            ++num_primes_;
            max_prime_ = i;
        }
    }
}

int PrimesSieve::num_digits(int num) {
    int digits = 0;
    while (num > 0) {
        num /= 10;
        ++digits;
    }
    return digits;
}

int main() {
    cout << "**************************** " <<  "Sieve of Eratosthenes" <<
            " ****************************" << endl;
    cout << "Search for primes up to: ";
    string limit_str;
    cin >> limit_str;
    int limit;

    // Use stringstream for conversion. Don't forget to #include <sstream>
    istringstream iss(limit_str);

    // Check for error.
    if ( !(iss >> limit) ) {
        cerr << "Error: Input is not an integer." << endl;
        return 1;
    }
    if (limit < 2) {
        cerr << "Error: Input must be an integer >= 2." << endl;
        return 1;
    }

    PrimesSieve sieve(limit);
    sieve.display_primes();

    return 0;
}
