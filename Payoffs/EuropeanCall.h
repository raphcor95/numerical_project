#pragma once
#include "Payoff.h"

using namespace std;


class EuropeanCall: public Payoff{
    private:
        double strike;

    public:
        EuropeanCall(double strike);
        ~EuropeanCall();
        double operator()(const std::vector<double>& vecPath) override;
};