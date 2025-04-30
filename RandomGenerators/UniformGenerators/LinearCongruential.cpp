#include "LinearCongruential.h"

LinearCongruential::LinearCongruential(
	myLong Multiplier, 
	myLong Increment, 
	myLong Modulus, 
	myLong Seed) :
	PseudoGenerator(Seed),
	Multiplier(Multiplier),
	Increment(Increment),
	Modulus(Modulus)
{
}

LinearCongruential::LinearCongruential() : Multiplier(40014),
Increment(0), Modulus(2147483563), PseudoGenerator(1) 
{
}

LinearCongruential::~LinearCongruential()
{
}

double LinearCongruential::Generate()
{
	Current = (Multiplier * Current + Increment) % Modulus;
	double result = (double) Current / Modulus;

	return result;
}

void bla()
{
	LinearCongruential X = LinearCongruential(1, 1, 1, 1);
}