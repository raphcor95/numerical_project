#pragma once

#include <iostream>
#include <fstream>
#include "Matrix.h"

class Input
{

    public:

        // Constructor
        Input();

        // Destructor
        ~Input();

        // Public Methods
        Matrix* CSV2Mat(const std::string& filepath);

};