#pragma once
#include "Underlying.h"

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
        virtual double Price() = 0;
};