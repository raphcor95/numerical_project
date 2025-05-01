#include "Input.h"
#include "Matrix.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cctype>
#include <typeinfo>


// -----------------
// HELPER 
// -----------------

// Helper function to trim whitespace
std::string trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) start++;
    
    auto end = s.end();
    do {
        --end;
    } while (start != end && std::isspace(*end));
    
    return std::string(start, end + 1);
}


// -----------------
// INPUT
// -----------------

// Constructor
Input::Input()
{

}

// Destructor
Input::~Input()
{

}


// -------------------------------------
// Methods 
// -------------------------------------

// Function to read CSV and return a pointer to a Matrix object
Matrix* Input::CSV2Mat(const std::string& filepath) {
    std::vector< std::vector<double> > data;
    std::ifstream file(filepath);
    
    // Check if the file was opened successfully
    if (!file.is_open()) {
        throw std::runtime_error("Error: could not open file: " + filepath);
    }

    std::string line;
    int row_index = 0;

    while (std::getline(file, line)) {  // Loop through lines (rows)
        std::vector<double> row;
        std::stringstream ss(line);
        std::string cell;
        int col_index = 0;

        while (std::getline(ss, cell, ',')) {  // Loop through columns in each row
            // Trim whitespace and check for any hidden characters
            cell = trim(cell);
            std::cout << "[" << cell << "]" << std::endl;
            for (char c : cell) {
                std::cout << "'" << c << "'(" << static_cast<int>(c) << ") ";
            }
            std::cout << std::stod(cell) << " is of type " << typeid(cell).name() << std::endl;

            try {
                // Convert to double and add to the row
                row.push_back(std::stod(cell));
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid value encountered in row " << row_index 
                          << ", column " << col_index << ": '" << cell << "'\n";
                throw std::runtime_error("Error: non-numeric value encountered.");
            } catch (const std::out_of_range& e) {
                std::cerr << "Out-of-range value encountered in row " << row_index
                          << ", column " << col_index << ": '" << cell << "'\n";
                throw std::runtime_error("Error: numeric value out of range.");
            }
            col_index++;
        }

        // Add the row to the matrix if it's not empty
        if (!row.empty()) {
            data.push_back(row);
        }
        row_index++;
    }

    // Create the matrix from the read data
    Matrix* mat = new Matrix(data);
    
    // Close the file
    file.close();

    // Return the matrix pointer
    return mat;

}

int main()
{
    std::string filepath = "../Inputs/matCorr.csv";
    Input* Inp = new Input();
    Matrix* Mat = Inp->CSV2Mat(filepath);
    Mat->print();
}