#include "Underlying.h"

// Constructor
Underlying::Underlying(RandomProcess* generator, 
                            double initVal, const std::vector<double>& vecWeights):
    Generator(generator), InitVal(initVal), VecWeights(vecWeights)
{

}

// Destructor
Underlying::~Underlying()
{

}

// Wrapper for simulation function
void Underlying::Simulate(double startTime, double endTime, size_t nbSteps)
{

}

// Return Path Method
SinglePath* Underlying::ReturnPath()
{
    return Path;
}