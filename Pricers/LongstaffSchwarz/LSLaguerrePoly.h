#pragma once
#include "LongstaffSchwarz.h"

class LSLaguerrePoly : public LongstaffSchwarz
{
    public:
        LSLaguerrePoly(Underlying* undl, std::vector<double> times,
            double nbSim, double startTime, double endTime, size_t nbSteps);
        ~LSLaguerrePoly();
        double Price(Payoffs* payoff);
};