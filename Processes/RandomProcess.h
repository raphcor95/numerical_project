#pragma once

#include "SinglePath.h"
#include "../RandomGenerators/ContinuousGenerators/Normal.h"

class RandomProcess
{
protected:
	RandomGenerator* Generator;
	vector<SinglePath*> Paths;
	int Dimension;
 
public:
	RandomProcess(RandomGenerator* Gen, int dim);
	virtual void Simulate(double startTime, double endTime, size_t nbSteps) = 0;
	// virtual void Simulate(double startTime, double endTime, size_t nbSteps, 
	// 						std::vector< std::vector<double> >* vecW_anti = nullptr) = 0;
	SinglePath* GetPath(int dimension);
	~RandomProcess();
};

