#include <iostream>
#include <vector>
#include "../RandomGenerators/UniformGenerators/EcuyerCombined.h"
#include "../RandomGenerators/UniformGenerators/LinearCongruential.h"
#include "QuasiRandomGenerators/QuasiRandomNormal/QuasiRandomNormal.h"
#include "../Processes/BSEuler1D.h"
#include "../Processes/BSEulerND.h"
#include "../Processes/Underlyings/Basket.h"
#include "../RandomGenerators/QuasiRandomGenerators/LDSequences/HaltonVdC.h"
#include "QuasiRandomGenerators/QuasiRandomNormal/QuasiRandomNormal.h"
#include "../Utils/Input.h"
#include "../Utils/Output.h"


/* 1-Dimensional BS Paths */

// Generation using Pseudo Random Numbers
void GenerateBSEuler1D(Normal* Norm,
                            double spot, double rate, double vol, double nbSim,
                            double startTime, double endTime, size_t nbSteps)
{
    RandomProcess* BSPath = new BSEuler1D(Norm, spot, rate, vol);
    std::vector<std::vector<double>> paths(nbSim, std::vector<double>(nbSteps));
    for (size_t i = 0; i < nbSim; i++)
    {
        BSPath->Simulate(startTime, endTime, nbSteps);
        paths[i] = BSPath->GetPath(0)->GetValues();
    }

    // Output the simulations
    Output* Out = new Output();
    Out->Vec2CSV(paths, "Outputs/PseudoBSEuler1D_Simulations.csv");
    std::cout << "Outputting the results in: Outputs/PseudoBSEuler1D_Simulations.csv" << std::endl;
    delete Out;
}

// Generation using Quasi Random Numbers
void GenerateQuasiBSEuler1D(QuasiRandomNormal* QuasiNormal,
                            double spot, double rate, double vol, double nbSim,
                            double startTime, double endTime, size_t nbSteps)
{
    RandomProcess* BSPath = new BSEuler1D(QuasiNormal, spot, rate, vol);
    std::vector<std::vector<double>> paths(nbSim, std::vector<double>(nbSteps));
    for (size_t i = 0; i < nbSim; i++)
    {
        BSPath->Simulate(startTime, endTime, nbSteps);
        paths[i] = BSPath->GetPath(0)->GetValues();
    }

    // Output the simulations
    Output* Out = new Output();
    Out->Vec2CSV(paths, "Outputs/QuasiBSEuler1D_Simulations.csv");
    std::cout << "Outputting the results in: Outputs/QuasiBSEuler1D_Simulations.csv" << std::endl;
    delete Out;
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

    // Simulate multiple trajectories
    for (size_t i = 0; i < 10; i++)
    {
        BSEulerMulti->Simulate(startTime, endTime, nbSteps);
    }

    // Get the trajectories
    for (size_t i = 0; i < paths.size(); i++)
    {
        // std::cout << i << "-th asset: ";
        paths[i] = BSEulerMulti->GetPath(i)->GetValues();
    }

    // Output the simulations
    Output* Out = new Output();
    Out->Vec2CSV(paths, "Outputs/BSEulerND_Simulations.csv");
    std::cout << "Outputting the results in: Outputs/BSEulerND_Simulations.csv" << std::endl;
    delete Out;
}     

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

    // Simulate multiple trajectories
    for (size_t i = 0; i < 10; i++)
    {
    BSEulerMulti->Simulate(startTime, endTime, nbSteps);
    }

    // Get the trajectories
    for (size_t i = 0; i < paths.size(); i++)
    {
    // std::cout << i << "-th asset: ";
    paths[i] = BSEulerMulti->GetPath(i)->GetValues();
    }

    // Output the simulations
    Output* Out = new Output();
    Out->Vec2CSV(paths, "Outputs/BSEulerND_Simulations.csv");
    std::cout << "Outputting the results in: Outputs/BSEulerND_Simulations.csv" << std::endl;
    delete Out;
}     

void GenerateBlackScholesBasket(Normal* Norm,
                                std::vector<double>& vecSpots, 
                                std::vector<double>& vecRates,
                                std::vector<double>& vecWeights,
                                Matrix* matCov,
                                double startTime, double endTime, size_t nbSteps)
{
    // Generator
    RandomProcess* BSEulerBasket = new BSEulerND(Norm, vecSpots, vecRates, matCov);
    Underlying* myBasket = new Basket(BSEulerBasket, 100.0, vecWeights);

    // Simulate the basket paths
    myBasket->Simulate(startTime, endTime, nbSteps);

    // Output the basket path
    SinglePath* Path = myBasket->ReturnPath();
    std::vector< std::vector<double> >vecOut(1, std::vector<double>(nbSteps, 0.0));
    vecOut[0] = Path->GetValues();
    Output* Out = new Output();
    Out->Vec2CSV(vecOut, "Outputs/BSBasket_Simulation.csv");
    std::cout << "Outputting the results in: Outputs/BSBasket_Simulation.csv" << std::endl;
    delete Out;
}


int main()
{
    // Variables
    double rate = 0.05;
    double vol = 0.1;
    double spot = 100;
    double nbSim = 1000;
    double startTime = 0;
    double endTime = 1;
    size_t nbSteps = 365;

    // Pseudo Generators
    UniformGenerator* Unif = new EcuyerCombined();
    Normal* Norm = new NormalBoxMuller(0.0, 1.0, Unif);

    /* --- 1-Dimensional BS Paths  --- */
    int dimension = 1;
    LDSequence* LDS1D = new HaltonVdC(dimension);
    QuasiRandomNormal* QuasiNormal1D = new NormInvCDF(0.0, 1.0, LDS1D);
    GenerateBSEuler1D(Norm, spot, rate, vol, nbSim, startTime, endTime, nbSteps);
    GenerateQuasiBSEuler1D(QuasiNormal1D, spot, rate, vol, nbSim, startTime, endTime, nbSteps);
    

    /* N-Dimensional BS Paths */
    // double N = 3.0;
    // std::vector<double> vecSpots(3, 100.0);
    // std::vector<double> vecRates(3, 0.05);
    // Input* Inp = new Input();
    // Matrix* matCov = Inp->CSV2Mat("Inputs/matCov.csv");
    // GenerateNDBlackScholes(Norm, vecSpots, vecRates, matCov, startTime, endTime, nbSteps);

    /* Basket Path */
    // std::vector<double> vecWeights = {0.0, 1.0, 0.0};
    // GenerateBlackScholesBasket(Norm, vecSpots, vecRates, vecWeights, matCov,
    //                             startTime, endTime, nbSteps);

    std::cout << "Test completed successfully!" << std::endl;

    return 0; 
}