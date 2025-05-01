#include <iostream>
#include <vector>
#include "../RandomGenerators/UniformGenerators/EcuyerCombined.h"
#include "../RandomGenerators/UniformGenerators/LinearCongruential.h"
#include "../Processes/BSEuler1D.h"

int main()
{
    // Variables
    double rate = 0.05;
    double vol = 0.1;
    double spot = 100;
    double nbSim = 100;
    double startTime = 0;
    double endTime = 1;
    size_t nbSteps = 10;

    // Generators
    UniformGenerator* Unif = new EcuyerCombined();
    Normal* Norm = new NormalBoxMuller(0.0, 1.0, Unif);

    /* 1-Dimensional BS Paths */
    RandomProcess* BSPath = new BSEuler1D(Norm, spot, rate, vol);
    std::vector<std::vector<double>> paths(nbSim, std::vector<double>(nbSteps));
    for (size_t i = 0; i < nbSim; i++)
    {
        BSPath->Simulate(startTime, endTime, nbSteps);
        paths[i] = BSPath->GetPath(0)->GetValues();
        
        // Print the results
        std::cout << i << "-th simulation ";
        for (size_t j = 0; j < paths[i].size(); j++)
        {
            std::cout << paths[i][j] << " - ";
        }
        std::cout << " " << std::endl;
    }



}