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

// Return Path Method
SinglePath* Underlying::ReturnPath()
{
    return Path;
}