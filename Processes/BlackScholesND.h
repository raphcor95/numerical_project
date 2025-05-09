#pragma once
#include <vector>
#include "RandomProcess.h"
#include "QuasiRandomGenerators/QuasiRandomNormal/QuasiRandomNormal.h"
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
        BlackScholesND(QuasiRandomNormal* Gen,
                        const std::vector<double>& vecSpots,
                        const std::vector<double>& vecRates,
                        Matrix* matCov);
        ~BlackScholesND();

        // Getters
        std::vector<double> GetSpots() const;
        std::vector<double> GetRates() const;
        Matrix* GetMatCov() const;
};

