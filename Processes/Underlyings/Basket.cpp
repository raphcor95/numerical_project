#include "Basket.h"
#include "Output.h"
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
    // Variables
    std::vector< std::vector<double> > vecDiff(VecWeights.size(), std::vector<double>(nbSteps, 0.0));
    double currVal = 0.0;

    // Diffusing the components
    Generator->Simulate(startTime, endTime, nbSteps);

    // Initialise the current value and the SinglePath
    for (size_t k = 0; k < VecWeights.size(); k++)
    {
        currVal += VecWeights[k] * Generator->GetPath(k)->GetValue(0);
    }

    // Initialise the Path
    std::cout << "[Basket] Checking if the Path pointer is null." << std::endl;
    if (Path != nullptr)            // Erase previous content if any
    {
        std::cout << "[Basket] Deleting Path: " << Path << std::endl;
        delete Path;
        Path = nullptr;
    }
    Path = new SinglePath(startTime, endTime, nbSteps);
    Path->AddValue(currVal);

    // Retrieve the trajectories
    for (size_t k = 0; k < VecWeights.size(); k++)
    {
        vecDiff[k] = Generator->GetPath(k)->GetValues();
    }

    // Output the trajectories
    // Output* Out = new Output();
    // Out->Vec2CSV(vecDiff, "Outputs/BSComponents_Simulations.csv");
    // std::cout << "Outputting the results in: Outputs/BSComponents_Simulations.csv" << std::endl;


    // Update the path of the Underlying
    for (size_t t = 0; t < nbSteps; t++)
    {  
        currVal = 0.0;

        // Compute the basket value
        for (size_t k = 0; k < VecWeights.size(); k++)
        {   
            currVal += VecWeights[k] * vecDiff[k][t+1];
        }
        Path->AddValue(currVal);
    }
}