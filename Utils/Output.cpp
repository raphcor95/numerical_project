#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Output.h"

// Default Constructor
Output::Output()
{
}

// Destructor
Output::~Output()
{

}

// Method to convert a matrix in to a csv file
void Output::Vec2CSV(
    const std::vector<std::vector<double>>& matrix, 
    const std::string& filename
)
{   
    // Create the file
    std::ofstream outFile(filename);

    // Check if the opening went smoothly
    if (!outFile)
    {
        std::cerr << "Error opening the file " << filename << std::endl;
        return;
    }

    for (const auto& row : matrix)
    {
        for (size_t i = 0; i < row.size(); i++)
        {
            outFile << row[i];
            if (i != row.size() - 1)
                outFile << ",";

        }
        outFile << "\n";
    } 
    outFile.close();
}
