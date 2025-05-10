#pragma once
#include "../Processes/Underlyings/Underlying.h"
#include "../Payoffs/Payoff.h"

class Pricer
{
    protected:
        Underlying* Undl;
        double NbSim;
        double StartTime;
        double EndTime;
        size_t NbSteps;
        double Rate;

    public:
        Pricer(Underlying* undl, double nbSim, double startTime, double endTime, size_t nbSteps,
                    double rate);
        ~Pricer();
        virtual double Price(Payoff* payoff, bool ControlVariate) = 0;
};