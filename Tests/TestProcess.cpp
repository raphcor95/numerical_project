#include <iostream>
#include <vector>
#include "../RandomGenerators/UniformGenerators/EcuyerCombined.h"
#include "../RandomGenerators/UniformGenerators/LinearCongruential.h"
#include "../Processes/BSEuler1D.h"
#include "../Processes/BSEulerND.h"
#include "../Utils/Input.h"
#include "../Utils/Output.h"


/* 1-Dimensional BS Paths */
void Generate1DBlackScholes(Normal* Norm,
                            double spot, double rate, double vol, double nbSim,
                            double startTime, double endTime, size_t nbSteps)
{
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

/* N-Dimensional BS Paths */
void GenerateNDBlackScholes(Normal* Norm,
                            std::vector<double>& vecSpots, 
                            std::vector<double>& vecRates,
                            Matrix* matCov,
                            double startTime, double endTime, size_t nbSteps)
{
    // Variables
    std::vector<std::vector<double>> paths(vecSpots.size(), std::vector<double>(nbSteps));
    RandomProcess* BSEulerMulti = new BSEulerND(Norm, vecSpots, vecRates, matCov);

    // Simulate the trajectories
    BSEulerMulti->Simulate(startTime, endTime, nbSteps);

    // Get the trajectories
    for (size_t i = 0; i < paths.size(); i++)
    {
        std::cout << i << "-th asset: ";
        paths[i] = BSEulerMulti->GetPath(i)->GetValues();
        for (size_t j = 0; j < paths[i].size(); j++)
        {
            std::cout << paths[i][j] << " ";
        }
        std::cout << " " << std::endl;
    }
}                            


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
   Generate1DBlackScholes(Norm, spot, rate, vol, nbSim, startTime, endTime, nbSteps);

    /* N-Dimensional BS Paths */
    double N = 3.0;
    std::vector<double> vecSpots(3, 100.0);
    std::vector<double> vecRates(3, 0.05);
    Input* Inp = new Input();
    Matrix* matCov = Inp->CSV2Mat("Inputs/matCov.csv");
    GenerateNDBlackScholes(Norm, vecSpots, vecRates, matCov, startTime, endTime, nbSteps);

    return 0; 
}