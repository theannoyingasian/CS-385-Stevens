/*******************************************************************************
* Filename: student.cpp
* Author : Himmat Garcha
* Version : 1.0
* Date : 9/12/2024
* Description: 
* Pledge : I pledge my honor that I have abided by the Stevens Honor System
******************************************************************************/
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

class Student{
    private:
        string first_;
        string last_; 
        float gpa_;
        int id_; 

    public: 
        Student(string first, string last, float gpa, int id)
            : first_(first), last_(last), gpa_(gpa), id_(id){}

            string fullname() const{
                return first_ + " " + last_;
            }

            float gpa() const{
                return gpa_;
            }
            
            int id() const{
                return id_;
            }

            void print_info() const{
                cout << fullname() << ", GPA: " << fixed << setprecision(2) << gpa_ << ", ID: " << id_ << endl;
            }
};



/**
* Takes a vector of Student objects, and returns a new vector
* with all Students whose GPA is < 1.0.
*/
vector<Student> find_failing_students(const vector<Student> &students) {
    vector<Student> failing_students;
        for( auto const &student : students){
            if(student.gpa() < 1.0){
                failing_students.push_back(student); 
            }
        }
    // Iterates through the students vector, appending each student whose gpa is
    // less than 1.0 to the failing_students vector.
    return failing_students;
}


/**
* Takes a vector of Student objects and prints them to the screen.
*/
void print_students(const vector<Student> &students) {
    for( auto const &students : students){
        students.print_info();
    }
    // Iterates through the students vector, calling print_info() for each student.
}



/**
* Allows the user to enter information for multiple students, then
* find those students whose GPA is below 1.0 and prints them to the
* screen. */
int main() {
    string first_name, last_name;
    float gpa;
    int id;
    char repeat;
    vector<Student> failing_students;
    vector<Student> students;
    do {
        cout << "Enter student's first name: ";
        cin >> first_name;
        cout << "Enter student's last name: ";
        cin >> last_name;
        gpa = -1;
        while (gpa < 0 || gpa > 4) {
            cout << "Enter student's GPA (0.0-4.0): ";
            cin >> gpa;
        }
        cout << "Enter student's ID: ";
        cin >> id;
        students.push_back(Student(first_name, last_name, gpa, id));
        cout << "Add another student to database (Y/N)? ";
        cin >> repeat;
    } while (repeat == 'Y' || repeat == 'y');
    cout << endl << "All students:" << endl;
    print_students(students);
    cout << endl << "Failing students:";
    failing_students = find_failing_students(students);
    if(failing_students.empty())
    {
        cout << " None" << endl;
    } else {
        cout << endl;
        for(auto const &students : failing_students){
          students.print_info();  
        }
    }
    return 0;
}