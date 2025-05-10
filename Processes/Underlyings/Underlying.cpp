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

// Get weights
std::vector<double>& Underlying::GetWeights()
{
    return VecWeights;
}

// Getter for the generator
Matrix* Underlying::GetMatCov() const
{
    return Generator->GetMatCov();
}

// Getter for the spots
std::vector<double> Underlying::GetSpots()
{
    return Generator->GetSpots();
}


// Return Path Method
SinglePath* Underlying::ReturnPath()
{
    return Path;
}

// Return vector simulations
std::vector< std::vector<double> >& Underlying::ReturnSimulations()
{
    return VecPaths;
}