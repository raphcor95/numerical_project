#pragma once
#include "Payoffs.h"

using namespace std;


class EuropeanCall: public Payoffs{
    private:
        double strike;

    public:
        EuropeanCall(double strike);
        ~EuropeanCall();
        double operator()(const std::vector<double>& vecPath) override;
};