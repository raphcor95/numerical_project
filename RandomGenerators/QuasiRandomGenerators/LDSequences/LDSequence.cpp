#include "LDSequence.h"
#include <iostream>


// Constructor
LDSequence::LDSequence(int dimension) : QuasiRandom(), Dimension(dimension)
{
    // Check the validity of the dimension
    if (dimension < 0)
    {
        throw std::runtime_error("Dimension must be strictly positive.");
    }
}

// Destructor
LDSequence::~LDSequence()
{

}