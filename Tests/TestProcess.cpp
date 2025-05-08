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
    std::cout << "[TestProcess] Generating 1 dimension Pseudo BS Euler processes." << std::endl;
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
    std::cout << "[TestProcess] Generating 1 dimension Quasi BS Euler processes." << std::endl;
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

// Generation using Pseudo Random Numbers
void GenerateBSEulerND(Normal* Norm,
                            std::vector<double>& vecSpots, 
                            std::vector<double>& vecRates,
                            Matrix* matCov,
                            double startTime, double endTime, size_t nbSteps)
{   
    std::cout << "[TestProcess] Generating N dimensions Pseudo BS Euler processes." << std::endl;

    // Variables
    std::vector<std::vector<double>> paths(vecSpots.size(), std::vector<double>(nbSteps));
    RandomProcess* BSEulerMulti = new BSEulerND(Norm, vecSpots, vecRates, matCov);

    // Simulate the trajectories
    BSEulerMulti->Simulate(startTime, endTime, nbSteps);

    // Get the trajectories
    for (size_t i = 0; i < paths.size(); i++)
    {
        // std::cout << i << "-th asset: ";
        paths[i] = BSEulerMulti->GetPath(i)->GetValues();
    }

    // Output the simulations
    Output* Out = new Output();
    Out->Vec2CSV(paths, "Outputs/PseudoBSEulerND_Simulations.csv");
    std::cout << "Outputting the results in: Outputs/PseudoBSEulerND_Simulations.csv" << std::endl;
    delete Out;
}   

// Generation using Quasi Random Numbers
void GenerateQuasiBSEulerND(QuasiRandomNormal* QuasiNormal,
                            std::vector<double>& vecSpots,
                            std::vector<double>& vecRates,
                            Matrix* matCov,
                            double startTime, double endTime, size_t nbSteps)
{
    std::cout << "[TestProcess] Generating N dimensions Quasi BS Euler processes." << std::endl;

    // Variables
    std::vector<std::vector<double>> paths(vecSpots.size(), std::vector<double>(nbSteps));
    RandomProcess* BSEulerMulti = new BSEulerND(QuasiNormal, vecSpots, vecRates, matCov);

    // Simulate the trajectories
    BSEulerMulti->Simulate(startTime, endTime, nbSteps);

    // Get the trajectories
    for (size_t i = 0; i < paths.size(); i++)
    {
    // std::cout << i << "-th asset: ";
    paths[i] = BSEulerMulti->GetPath(i)->GetValues();
    }

    // Output the simulations
    Output* Out = new Output();
    Out->Vec2CSV(paths, "Outputs/QuasiBSEulerND_Simulations.csv");
    std::cout << "Outputting the results in: Outputs/QuasiBSEulerND_Simulations.csv" << std::endl;
    delete Out;
}     


/* N-Dimensional BS Paths */

// Generation using Pseudo Random Numbers
void GenerateBasketBSEuler(Normal* Norm,
                                std::vector<double>& vecSpots, 
                                std::vector<double>& vecRates,
                                std::vector<double>& vecWeights,
                                Matrix* matCov, size_t nbSim,
                                double startTime, double endTime, size_t nbSteps)
{
    std::cout << "[TestProcess] Generating N Basket Pseudo BS Euler processes." << std::endl;

    // Vector to store the paths
    std::vector< std::vector<double> > vecPaths;

    // Generator
    RandomProcess* BSEulerBasket = new BSEulerND(Norm, vecSpots, vecRates, matCov);
    Underlying* myBasket = new Basket(BSEulerBasket, 100.0, vecWeights);

    // Generate all the paths
    for (size_t i = 0; i < nbSim; i++)
    {
        myBasket->Simulate(startTime, endTime, nbSteps);
        vecPaths.push_back(myBasket->ReturnPath()->GetValues());
    }

    // Output the basket path
    Output* Out = new Output();
    Out->Vec2CSV(vecPaths, "Outputs/PseudoBasketBSEuler_Simulations.csv");
    std::cout << "Outputting the results in: Outputs/PseudoBasketBSEuler_Simulations.csv" << std::endl;
    delete Out;
}

// Generation using Quasi Random Numbers
void GenerateBasketQuasiBSEuler(QuasiRandomNormal* QuasiNormal,
    std::vector<double>& vecSpots, 
    std::vector<double>& vecRates,
    std::vector<double>& vecWeights,
    Matrix* matCov, size_t nbSim,
    double startTime, double endTime, size_t nbSteps)
{

    std::cout << "[TestProcess] Generating N Basket Quasi BS Euler processes." << std::endl;

    // Vector to store the paths
    std::vector< std::vector<double> > vecPaths;

    // Generator
    RandomProcess* BSEulerBasket = new BSEulerND(QuasiNormal, vecSpots, vecRates, matCov);
    Underlying* myBasket = new Basket(BSEulerBasket, 100.0, vecWeights);

    // Generate all the paths
    for (size_t i = 0; i < nbSim; i++)
    {
        myBasket->Simulate(startTime, endTime, nbSteps);
        vecPaths.push_back(myBasket->ReturnPath()->GetValues());
    }

    // Output the basket path
    Output* Out = new Output();
    Out->Vec2CSV(vecPaths, "Outputs/QuasiBasketBSEuler_Simulations.csv");
    std::cout << "Outputting the results in: Outputs/QuasiBasketBSEuler_Simulations.csv" << std::endl;
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
    

    /* --- N-Dimensional BS Paths --- */
    double N = 3.0;
    std::vector<double> vecSpots(3, 100.0);
    std::vector<double> vecRates(3, 0.05);
    Input* Inp = new Input();
    Matrix* matCov = Inp->CSV2Mat("Inputs/matCov.csv");
    GenerateBSEulerND(Norm, vecSpots, vecRates, matCov, startTime, endTime, nbSteps);
    LDSequence* LDSND = new HaltonVdC(static_cast<int>(N));
    QuasiRandomNormal* QuasiNormalND = new NormInvCDF(0.0, 1.0, LDSND);
    GenerateQuasiBSEulerND(QuasiNormalND, vecSpots, vecRates, matCov, startTime, endTime, nbSteps);

    /* --- Basket BS Paths --- */
    std::vector<double> vecWeights = {1.0, 0.0, 0.0};
    LDSequence* LDBasket = new HaltonVdC(static_cast<int>(N));
    QuasiRandomNormal* QuasiNormalBasket = new NormInvCDF(0.0, 1.0, LDBasket);
    GenerateBasketBSEuler(Norm, vecSpots, vecRates, vecWeights, matCov, nbSim,
                                startTime, endTime, nbSteps);
    GenerateBasketQuasiBSEuler(QuasiNormalBasket, vecSpots, vecRates, vecWeights, matCov, nbSim,
        startTime, endTime, nbSteps);

    std::cout << "Test completed successfully!" << std::endl;

    return 0; 
}