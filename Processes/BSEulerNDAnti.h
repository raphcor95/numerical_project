#pragma once
#include "BlackScholesND.h"


class BSEulerNDAnti : public BlackScholesND
{
    protected:
        std::vector< std::vector<double> > VecW;
        bool NewGeneration;

    public:
        BSEulerNDAnti(Normal* gen,
            const std::vector<double>& vecSpots,
            const std::vector<double>& vecRates,
            Matrix* MatCov);

        BSEulerNDAnti(QuasiRandomNormal* gen,
            const std::vector<double>& vecSpots,
            const std::vector<double>& vecRates,
            Matrix* MatCov);

        void Simulate(double startTime, double endTime, size_t nbSteps);
        ~BSEulerNDAnti();

};