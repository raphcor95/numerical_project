#pragma once
#include "../Pricer.h"

/* This class implements LongstaffScharz algorithm with the Laguerre
polynomials from order 0 up to order 2, as suggested in the authors 
original paper for the American Put option pricing */

class LongstaffSchwarz : public Pricer
{
    protected:
        std::vector<double> VecTimes;
        std::vector<SinglePath*> VecPaths;

    public:
        LongstaffSchwarz(Underlying* undl, std::vector<double> times,
                        double nbSim, double startTime, double endTime, size_t nbSteps);
        ~LongstaffSchwarz();
        void Simulate();
        std::vector<SinglePath*> ReturnPaths();
};