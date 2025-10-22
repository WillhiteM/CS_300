//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Matthew Willhite
// Version     : 1.1
// Copyright   : Copyright © 2025 SNHU COCE
// Description : ABCU Course Planner - Loads course data from a CSV file,
//               stores it in a hash table, and allows users to view
//               an ordered course list or details on a single course.
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

//-------------------------------------------------------------
// Data structure representing a single course and its details
//-------------------------------------------------------------
struct Course {
    string courseNumber;            // Unique identifier (e.g., CSCI101)
    string name;                    // Full course title
    vector<string> prereqs;         // List of prerequisite course numbers
};

//-------------------------------------------------------------
// Global hash table keyed by courseNumber for O(1) average access
//-------------------------------------------------------------
unordered_map<string, Course> courseTable;

//-------------------------------------------------------------
// loadCourses()
// Reads a CSV file and populates the hash table with Course objects.
// Each line must contain at least a course number and name.
//-------------------------------------------------------------
void loadCourses(string filename) {
    courseTable.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file. Please check the path and try again." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        // Ignore empty lines
        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        vector<string> fields;

        // Split line by commas
        while (getline(ss, token, ',')) {
            fields.push_back(token);
        }

        // Skip malformed lines
        if (fields.size() < 2) continue;

        // Create a course object and assign fields
        Course c;
        c.courseNumber = fields[0];
        c.name = fields[1];

        // Add any remaining fields as prerequisites
        for (size_t i = 2; i < fields.size(); ++i) {
            if (!fields[i].empty()) {
                c.prereqs.push_back(fields[i]);
            }
        }

        // Store in hash table
        courseTable[c.courseNumber] = c;
    }

    file.close();
    cout << "Courses loaded successfully from " << filename << "." << endl;
}

//-------------------------------------------------------------
// printCourseList()
// Prints all courses in alphanumeric order by course number.
//-------------------------------------------------------------
void printCourseList() {
    if (courseTable.empty()) {
        cout << "No courses loaded. Please load the data first." << endl;
        return;
    }

    // Gather and sort all course numbers
    vector<string> keys;
    for (auto const& pair : courseTable) {
        keys.push_back(pair.first);
    }
    sort(keys.begin(), keys.end());

    cout << "\nHere is a sample schedule:\n" << endl;
    for (auto const& k : keys) {
        cout << k << ", " << courseTable[k].name << endl;
    }
    cout << endl;
}

//-------------------------------------------------------------
// printCourse()
// Displays a single course and its prerequisites.
//-------------------------------------------------------------
void printCourse(string number) {
    // Convert to uppercase for case-insensitive matching
    transform(number.begin(), number.end(), number.begin(), ::toupper);

    // Lookup the course in the hash table
    if (courseTable.find(number) == courseTable.end()) {
        cout << "Course not found. Please check the course number and try again." << endl;
        return;
    }

    Course c = courseTable[number];
    cout << c.courseNumber << ", " << c.name << endl;

    // Display prerequisites if any
    if (c.prereqs.empty()) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < c.prereqs.size(); ++i) {
            if (i > 0) cout << ", ";
            cout << c.prereqs[i];
        }
        cout << endl;
    }
}

//-------------------------------------------------------------
// displayMenu()
// Displays menu options clearly with spacing for readability.
//-------------------------------------------------------------
void displayMenu() {
    cout << "\n======================================" << endl;
    cout << "           ABCU Course Planner         " << endl;
    cout << "======================================" << endl;
    cout << "1. Load Data Structure" << endl;
    cout << "2. Print Course List" << endl;
    cout << "3. Print Course" << endl;
    cout << "9. Exit" << endl;
    cout << "--------------------------------------" << endl;
    cout << "Enter your choice: ";
}

//-------------------------------------------------------------
// main()
// Displays a simple text menu to load data, print the list,
// print an individual course, or exit.
//-------------------------------------------------------------
int main() {
    cout << "Welcome to the ABCU Course Planner!" << endl;
    bool loaded = false;   // Tracks whether data has been loaded
    int choice = 0;
    string filename;

    while (true) {
        displayMenu();
        if (!(cin >> choice)) {
            // Handle invalid input like letters
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 9.\n" << endl;
            continue;
        }
        cin.ignore();

        // Handle menu options
        if (choice == 1) {
            cout << "\nEnter the full path and file name to load (e.g., C:\\\\Users\\\\Name\\\\ABCU_Advising_Program_Input.csv): ";
            getline(cin, filename);
            loadCourses(filename);
            loaded = true;
        }
        else if (choice == 2) {
            if (!loaded) {
                cout << "Please load the data first (Option 1)." << endl;
            }
            else {
                printCourseList();
            }
        }
        else if (choice == 3) {
            if (!loaded) {
                cout << "Please load the data first (Option 1)." << endl;
            }
            else {
                string num;
                cout << "Enter the course number you want to know about (e.g., CSCI400): ";
                cin >> num;
                printCourse(num);
            }
        }
        else if (choice == 9) {
            cout << "Thank you for using the ABCU Course Planner! Goodbye!" << endl;
            break;
        }
        else {
            cout << choice << " is not a valid option. Please choose 1, 2, 3, or 9.\n" << endl;
        }
    }

    return 0;
}
