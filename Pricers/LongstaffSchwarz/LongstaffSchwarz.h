#pragma once
#include "../Pricer.h"
#include "Matrix.h"


class LongstaffSchwarz : public Pricer
{
    protected:
        std::vector<double> VecTimes;
        std::vector<SinglePath*> VecPaths;

    public:
        LongstaffSchwarz(Underlying* undl, std::vector<double> times,
                        double nbSim, double startTime, double endTime, size_t nbSteps,
                        double rate);
        virtual ~LongstaffSchwarz();
        void Simulate();
        virtual double ComputeExpectedValue(Matrix& MatColAlphas, double x) = 0;
        const std::vector<SinglePath*>& ReturnPaths() const;
};