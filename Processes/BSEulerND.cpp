#include "BSEulerND.h"
#include <vector>


// Constructor
BSEulerND::BSEulerND(
    Normal* Gen, 
    const std::vector<double>& vecSpots, 
    const std::vector<double>& vecRates,
    Matrix* matCorr
) : BlackScholesND(Gen, vecSpots, vecRates, matCorr)
{

}

// Constructor with QuasiRandomNormal
BSEulerND::BSEulerND(
    QuasiRandomNormal* Gen, 
    const std::vector<double>& vecSpots, 
    const std::vector<double>& vecRates,
    Matrix* matCorr
) : BlackScholesND(Gen, vecSpots, vecRates, matCorr)
{

}

// Destructor
BSEulerND::~BSEulerND()
{

}

// Simulating Method
void BSEulerND::Simulate(double startTime, double endTime, size_t nbSteps)
{
    // Variables 
    size_t N = this->VecSpots.size();                   // Number of underlyings
    double dt = (endTime - startTime) / nbSteps;        // Number of Steps
    std::vector<SinglePath*> vecPaths;                  // Vector of single paths
    std::vector<double> vecCurr(N, 0.0);                // Vector of current prices
    std::vector<double> vecW(N, 0.0);                   // Vector of Brownian Motions
    std::vector<double> vecR(N, 0.0);                   // Vector of Returns  
    double corrW = 0.0;                                 // Correlated Brownian

    // Get LU Decomposition of the Correlation Matrix
    std::pair<Matrix, Matrix> MatLU = this->MatCov->choleskyDecomposition();
    Matrix L = MatLU.first;

    // Fill the first values in the Paths with the spots values in vecSpots
    for (size_t i = 0; i < N; i++)
    {
        vecCurr[i] = this->VecSpots[i];
        SinglePath* Path = new SinglePath(startTime, endTime, nbSteps);
        Path->AddValue(vecCurr[i]);
        vecPaths.push_back(Path);
    }

    // Fill the rest of the vectors using Euler Disretised version of the BS dynamics
    for (size_t t = 0; t < nbSteps; t++)
    {

        // Generate Brownian Vector
        for (size_t k = 0; k < N; k++)
        {
            vecW[k] = Generator->Generate(); 
        }

        // Generate Correlated Paths
        for (size_t k = 0; k < N; k++)
        {
            // Compute correlated brownian
            corrW = 0.0;
            for (size_t j = 0; j < N; j++)
            {
                corrW += L(k, j) * vecW[j];
            }

            // Update current values
            vecCurr[k] = vecCurr[k] 
                    + VecRates[k] * vecCurr[k] * dt + vecCurr[k] * corrW * sqrt(dt);
            
            // Store the results in the vecPaths
            vecPaths[k]->AddValue(vecCurr[k]);
        }
    }

    // Store the values in the Paths member of the RandomProcess Mother Class
    // Clear previous paths
    // std::cout << "[BSEulerND] Checking if the vector of pointers is empty." << std::endl;
    // std::cout << "[BSEulerND] Size of the Paths vector: " << Paths.size() << std::endl;
    if (Paths.size() > 0)
    {
        for (auto p : Paths)
        {
            // std::cout << "[BSEuler] Deleting pointer: " << p << std::endl;
            delete p;
        }
    }
    Paths.clear();
    for (size_t k = 0; k < N; k++)
    {
        Paths.push_back(vecPaths[k]);
    }
}