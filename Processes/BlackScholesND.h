#pragma once
#include <vector>
#include "RandomProcess.h"
#include "../Utils/Matrix.h"

class BlackScholesND : public RandomProcess
{
    protected:
        const std::vector<double>& VecSpots;
        const std::vector<double>& VecRates;
        Matrix* MatCov;

    public:
        BlackScholesND(Normal* Gen, 
                        const std::vector<double>& vecSpots,
                        const std::vector<double>& vecRates,
                        Matrix* matCov);
        ~BlackScholesND();
};

