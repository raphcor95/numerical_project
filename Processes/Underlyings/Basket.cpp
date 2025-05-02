#include "Basket.h"
#include <iostream>

// Constructor
Basket::Basket(
    RandomProcess* generator, 
    double initVal,
    const std::vector<double>& vecWeights
) : 
    Underlying(generator, initVal, vecWeights)
{

}

// Destructor
Basket::~Basket()
{

}

// Simulation Method
void Basket::Simulate(double startTime, double endTime, size_t nbSteps)
{
    std::cout << "Not implemented yet" << std::endl;
}