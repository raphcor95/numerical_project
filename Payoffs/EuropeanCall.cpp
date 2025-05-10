#include "EuropeanCall.h"

using namespace std;


EuropeanCall::EuropeanCall(double strike) : Payoff("Call"), strike(strike) {}

EuropeanCall::~EuropeanCall() {}

double EuropeanCall::operator()(const std::vector<double>& vecPath) {
    double ST = vecPath.back();
    return std::max(ST - strike, 0.0);
}

const double EuropeanCall::GetStrike()
{
    return strike;
}
