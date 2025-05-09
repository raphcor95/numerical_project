#pragma once
#include "Matrix.h"
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
	virtual Matrix* GetMatCov() const {return nullptr;}
	virtual std::vector<double> GetSpots() const {return {};}
	virtual void Simulate(double startTime, double endTime, size_t nbSteps) = 0;
	// virtual void Simulate(double startTime, double endTime, size_t nbSteps, 
	// 						std::vector< std::vector<double> >* vecW_anti = nullptr) = 0;
	SinglePath* GetPath(int dimension);
	~RandomProcess();
};

