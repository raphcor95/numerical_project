#pragma once
#include "LongstaffSchwarz.h"
#include "Matrix.h"

class LSStandard : public LongstaffSchwarz
{
    protected: 
        int Order;

    public:
        LSStandard(Underlying* undl, std::vector<double> times,
            double nbSim, double startTime, double endTime, size_t nbSteps, int order, double rate);
        ~LSStandard();
        double Price(Payoff* payoff, bool ControlVariate);
    
    private:
        double ComputeExpectedValue(Matrix& MatColAlphas, double x);
};