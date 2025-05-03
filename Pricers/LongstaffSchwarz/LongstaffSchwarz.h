#pragma once
#include "../Pricer.h"


class LongstaffSchwarz : public Pricer
{
    protected:
        std::vector<double> Times;
        std::vector<SinglePath*> Paths;

    public:
        LongstaffSchwarz(Underlying* undl, std::vector<double> times,
                        double nbSim, double startTime, double endTime, size_t nbSteps);
        ~LongstaffSchwarz();
        void Simulate();
        std::vector<SinglePath*> ReturnPaths();
};