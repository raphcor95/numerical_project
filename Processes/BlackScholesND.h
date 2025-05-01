#pragma once
#include <vector>
#include "RandomProcess.h"

class BlackScholesND : public RandomProcess
{
    protected:
        std::vector<double> VecSpots;
        std::vector<double> VecRates;
        std::vector<std::vector<double>> MatCorr;

    public:
        BlackScholesND(Normal* Gen, 
                        std::vector<double>& vecSpots,
                        std::vector<double>& vecRates,
                        std::vector<std::vector<double>>& matCorr);
        ~BlackScholesND();
};

