#include "SinglePath.h"
#include <stdexcept>
#include <iostream>

const double eps = 1e-10;

SinglePath::SinglePath(double start, double end, size_t nbSteps) : 
	StartTime(start), 
	EndTime(end), 
	NbSteps(nbSteps)
{
	if (nbSteps == 0)
	{
		throw runtime_error("Nb Steps is zero");
	}
	timeStep = (EndTime - StartTime) / nbSteps;
}

void SinglePath::AddValue(double val)
{
	Values.push_back(val);
	if (Times.size() == 0)
	{
		Times.push_back(StartTime);
	}
	else
	{
		Times.push_back(Times.back() + timeStep); // last time + time step
	}
}

double SinglePath::GetValue(double time)
{
	double result = 0.;
	if (time <= StartTime)
	{
		result = Values[0];
	}
	else if (time >= EndTime)
	{
		result = Values.back();
	}
	else
	{
		for (size_t i = 1; i < Times.size(); ++i)
		{
			if (Times[i] < time)
				continue;
			else if (Times[i] == time)
			{
				result = Values[i];
				break;
			}
			else
			{
				double upperTime = Times[i];
				double lowerTime = Times[i - 1];
				
				double upperValue = Values[i];
				double lowerValue = Values[i - 1];

				result = lowerValue * ((upperTime - time) / (upperTime - lowerTime))
					+
					upperValue * ((time - lowerTime) / (upperTime - lowerTime));
				break;
			}
		}
	}

	return result;
}

vector<double>& SinglePath::GetValues()
{
	return Values;
}

// Method to return part of the Values (up to a given point in time)
vector<double> SinglePath::GetValuesUpToT(double t)
{	
	std::cout << "[SinglePath] Checking the validity of the input." << std::endl;
	if (t > Times.back() + eps || t < Times.front() - eps)
	{
		throw std::runtime_error("Time must belong to the range ["
								+ std::to_string(Times.front())
								+ " : "
								+ std::to_string(Times.back()) + ".");
	}

	// Variables
	double terminalValue = GetValue(t);
	std::vector<double> vecResults;

	// Building the truncated vector
	for (size_t i = 0; i < Values.size(); i++)
	{
		if (Times[i] < t)
		{
			vecResults.push_back(Values[i]);
		} else {
			vecResults.push_back(terminalValue);
			break;
		}
	}
	return vecResults;
}


vector<double>& SinglePath::GetTimes()
{
	return Times;
}

SinglePath::~SinglePath()
{
}