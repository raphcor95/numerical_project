#pragma once
#include <vector>
class Payoffs
{

public:
    Payoffs();
	~Payoffs();
    virtual double operator()(const std::vector<double>& vecPath) = 0;
};