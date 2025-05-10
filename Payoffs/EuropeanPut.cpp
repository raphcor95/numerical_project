#include "EuropeanPut.h"

using namespace std;


EuropeanPut::EuropeanPut(double strike) : Payoff("Put"), strike(strike) {}

EuropeanPut::~EuropeanPut() {}

double EuropeanPut::operator()(const std::vector<double>& vecPath) {
    double ST = vecPath.back();
    return std::max(strike - ST, 0.0);
}

const double EuropeanPut::GetStrike()
{
    return strike;
}
