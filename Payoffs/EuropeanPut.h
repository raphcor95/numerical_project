#pragma once
#include "Payoff.h"

using namespace std;


class EuropeanPut: public Payoff{
    private:
        double strike;

    public:
        EuropeanPut(double strike);
        ~EuropeanPut();
        double operator()(const std::vector<double>& vecPath) override;
        const double GetStrike();
};