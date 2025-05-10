#include "BlackScholesND.h"
#include "../Utils/Matrix.h"
#include <vector>
#include <string>
#include <iostream>

BlackScholesND::BlackScholesND(Normal* Gen,
    const std::vector<double>& vecSpots,
    const std::vector<double>& vecRates,
    Matrix* matCov
) : 
    RandomProcess(Gen, vecSpots.size()),
    VecSpots(vecSpots),
    VecRates(vecRates),
    MatCov(matCov)
{
    // Check for the validity of the inputs
    if (vecSpots.size() != vecRates.size())
    {
        throw runtime_error("Error: Lengths must match. vecSpots has length: " 
                            + std::to_string(vecSpots.size()) 
                            + " and vecRates has length " + std::to_string(vecRates.size()) + "."
        );
    } 
    
    if (MatCov->getRows() != MatCov->getCols())
    {
        throw runtime_error("Error: Correlation matrix must be a square matrix. Dimensions are" 
                            + std::to_string(MatCov->getRows())
                            + "x"
                            + std::to_string(MatCov->getCols())
        );
    }

    if (vecSpots.size() != MatCov->getRows())
    {
        throw std::runtime_error("Error: Length of the vecSpots vector "
                                + std::to_string(vecSpots.size())
                                + " must match the correlation matrix dimensions "
                                + std::to_string(MatCov->getRows())
                                + " square matrix."
        );
    }

    if (vecRates.size() != MatCov->getRows())
    {
        throw std::runtime_error("Error: Length of the vecRates vector "
                                + std::to_string(vecRates.size())
                                + " must match the correlation matrix dimensions "
                                + std::to_string(MatCov->getRows())
                                + " square matrix."
        );
    }
}

// Constructor with QuasiRandomNormal
BlackScholesND::BlackScholesND(QuasiRandomNormal* Gen,
    const std::vector<double>& vecSpots,
    const std::vector<double>& vecRates,
    Matrix* matCov
) : 
    RandomProcess(Gen, vecSpots.size()),
    VecSpots(vecSpots),
    VecRates(vecRates),
    MatCov(matCov)
{
    // Check for the validity of the inputs
    if (vecSpots.size() != vecRates.size())
    {
        throw runtime_error("Error: Lengths must match. vecSpots has length: " 
                            + std::to_string(vecSpots.size()) 
                            + " and vecRates has length " + std::to_string(vecRates.size()) + "."
        );
    } 
    
    if (MatCov->getRows() != MatCov->getCols())
    {
        throw runtime_error("Error: Correlation matrix must be a square matrix. Dimensions are" 
                            + std::to_string(MatCov->getRows())
                            + "x"
                            + std::to_string(MatCov->getCols())
        );
    }

    if (vecSpots.size() != MatCov->getRows())
    {
        throw std::runtime_error("Error: Length of the vecSpots vector "
                                + std::to_string(vecSpots.size())
                                + " must match the correlation matrix dimensions "
                                + std::to_string(MatCov->getRows())
                                + " square matrix."
        );
    }

    if (vecRates.size() != MatCov->getRows())
    {
        throw std::runtime_error("Error: Length of the vecRates vector "
                                + std::to_string(vecRates.size())
                                + " must match the correlation matrix dimensions "
                                + std::to_string(MatCov->getRows())
                                + " square matrix."
        );
    }
}

// Getters
std::vector<double> BlackScholesND::GetRates() const 
{
    return VecRates;
}

Matrix* BlackScholesND::GetMatCov() const
{
    return MatCov;
}

std::vector<double> BlackScholesND::GetSpots() const
{
    return VecSpots;
}

BlackScholesND::~BlackScholesND()
{

}