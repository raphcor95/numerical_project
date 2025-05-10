#pragma once
#include <vector>
class Payoff
{

public:
    Payoff(std::string name);
	~Payoff();
    virtual double operator()(const std::vector<double>& vecPath) = 0;
    virtual const double GetStrike() = 0; 
    std::string Name;
};