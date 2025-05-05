#pragma once
#include "LongstaffSchwarz.h"
#include "Matrix.h"

class LSLaguerrePoly : public LongstaffSchwarz
{
    public:
        LSLaguerrePoly(Underlying* undl, std::vector<double> times,
            double nbSim, double startTime, double endTime, size_t nbSteps);
        ~LSLaguerrePoly();
        double Price(Payoff* payoff);
    
    private:
        double Order0Poly(double x);
        double Order1Poly(double x);
        double Order2Poly(double x);
        double ComputeExpectedValue(Matrix& MatColAlphas, double x);
};