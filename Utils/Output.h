#pragma once

#include <iostream>
#include <fstream>
#include <iostream>

class Output
{

    public:

        // Constructor
        Output();

        // Destructor
        ~Output();

        // Public Methods
        void Vec2CSV(
            const std::vector<std::vector<double>>& Matrix, 
            const std::string& filename);
        
};