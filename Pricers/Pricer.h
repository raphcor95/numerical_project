#pragma once
#include "../Processes/Underlyings/Underlying.h"
#include "../Payoffs/Payoffs.h"

class Pricer
{
    protected:
        Underlying* Undl;
        double NbSim;
        double StartTime;
        double EndTime;
        size_t NbSteps;

    public:
        Pricer(Underlying* undl, double nbSim, double startTime, double endTime, size_t nbSteps);
        ~Pricer();
        virtual double Price(Payoffs* payoff) = 0;
};