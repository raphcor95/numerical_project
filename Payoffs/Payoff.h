#pragma once
#include <vector>
class Payoff
{

public:
    Payoff();
	~Payoff();
    virtual double operator()(const std::vector<double>& vecPath) = 0;
    virtual const double GetStrike() = 0; 
};