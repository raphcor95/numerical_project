#include "BlackScholesND.h"
#include <vector>

BlackScholesND::BlackScholesND(Normal* Gen,
    std::vector<double>& vecSpots,
    std::vector<double>& vecRates,
    std::vector<std::vector<double>>& matCorr
) : 
    RandomProcess(Gen, vecSpots.size()),
    VecSpots(vecSpots),
    VecRates(vecRates),
    MatCorr(matCorr)
{

}

BlackScholesND::~BlackScholesND()
{
    
}