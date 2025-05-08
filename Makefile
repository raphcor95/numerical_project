# -*- Makefile -*-

# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall \
			   -I./Utils \
               -I./RandomGenerators \
               -I./RandomGenerators/UniformGenerators \
			   -I./Pricers/PDE \
			   -I./Pricers \
			   -I./Processes \
			   -I./Processes/Underlyings


all: test_random test_pde test_proc test_mc


# ------------------------------
# Tests
# ------------------------------

# Generators -------------------------------------------
GEN_SRCS = Tests/TestRandom.cpp \
		   RandomGenerators/RandomGenerator.cpp \
           RandomGenerators/UniformGenerators/EcuyerCombined.cpp \
           RandomGenerators/UniformGenerators/LinearCongruential.cpp \
           RandomGenerators/UniformGenerators/PseudoGenerator.cpp \
           RandomGenerators/UniformGenerators/UniformGenerator.cpp \
           RandomGenerators/DiscreteGenerators/Bernoulli.cpp \
           RandomGenerators/DiscreteGenerators/Binomial.cpp \
           RandomGenerators/DiscreteGenerators/FiniteSet.cpp \
           RandomGenerators/DiscreteGenerators/HeadTail.cpp \
           RandomGenerators/DiscreteGenerators/Poisson.cpp \
           RandomGenerators/DiscreteGenerators/DiscreteGenerator.cpp \
           RandomGenerators/ContinuousGenerators/ContinuousGenerator.cpp \
           RandomGenerators/ContinuousGenerators/Normal.cpp \
		   RandomGenerators/ContinuousGenerators/Exponential.cpp \
		   RandomGenerators/QuasiRandomGenerators/QuasiRandom.cpp \
		   RandomGenerators/QuasiRandomGenerators/LDSequences/LDSequence.cpp \
		   RandomGenerators/QuasiRandomGenerators/LDSequences/HaltonVdC.cpp \
		   RandomGenerators/QuasiRandomGenerators/QuasiRandomNormal/QuasiRandomNormal.cpp \
		   Utils/Output.cpp  \
		   Utils/Tools.cpp

GEN_OBJS = $(GEN_SRCS:.cpp=.o)

test_random: $(GEN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(GEN_OBJS)


# PDE --------------------------------------------------
PDE_SRCS = Tests/TestPDE.cpp \
			Pricers/PDE/PDEGrid2D.cpp \
			Pricers/PDE/R1R1Function.cpp \
			Pricers/PDE/R2R1Function.cpp \
			Utils/Output.cpp

PDE_OBJS = $(PDE_SRCS:.cpp=.o)

test_pde: $(PDE_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(PDE_OBJS)



# Processes ----------------------------------------------
PROC_SRCS = Tests/TestProcess.cpp \
			RandomGenerators/RandomGenerator.cpp \
			RandomGenerators/UniformGenerators/EcuyerCombined.cpp \
			RandomGenerators/UniformGenerators/LinearCongruential.cpp \
			RandomGenerators/UniformGenerators/PseudoGenerator.cpp \
			RandomGenerators/UniformGenerators/UniformGenerator.cpp \
			RandomGenerators/ContinuousGenerators/ContinuousGenerator.cpp \
			RandomGenerators/ContinuousGenerators/Normal.cpp \
			Processes/SinglePath.cpp \
			Processes/RandomProcess.cpp \
			Processes/BSEuler1D.cpp \
			Processes/BlackScholes1D.cpp \
			Processes/BSEulerND.cpp \
			Processes/BlackScholesND.cpp \
			Processes/Underlyings/Underlying.cpp \
			Processes/Underlyings/Basket.cpp \
			Utils/Input.cpp \
			Utils/Output.cpp \
			Utils/Matrix.cpp

PROC_OBJS = $(PROC_SRCS:.cpp=.o)

test_proc: $(PROC_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(PROC_OBJS)


# Bermundan Pricing -----------------------------------------
LS_SRCS = Tests/TestLS.cpp \
			RandomGenerators/RandomGenerator.cpp \
			RandomGenerators/UniformGenerators/EcuyerCombined.cpp \
			RandomGenerators/UniformGenerators/LinearCongruential.cpp \
			RandomGenerators/UniformGenerators/PseudoGenerator.cpp \
			RandomGenerators/UniformGenerators/UniformGenerator.cpp \
			RandomGenerators/ContinuousGenerators/ContinuousGenerator.cpp \
			RandomGenerators/ContinuousGenerators/Normal.cpp \
			Processes/SinglePath.cpp \
			Processes/RandomProcess.cpp \
			Processes/BSEulerND.cpp \
			Processes/BlackScholesND.cpp \
			Processes/Underlyings/Underlying.cpp \
			Processes/Underlyings/Basket.cpp \
			Pricers/Pricer.cpp \
			Pricers/LongstaffSchwarz/LongstaffSchwarz.cpp \
			Pricers/LongstaffSchwarz/LSLaguerrePoly.cpp \
			Payoffs/Payoff.cpp \
			Payoffs/EuropeanCall.cpp \
			Utils/Input.cpp \
			Utils/Output.cpp \
			Utils/Matrix.cpp

			
LS_OBJS = $(LS_SRCS:.cpp=.o)

test_ls: $(LS_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(LS_OBJS)

# Monte Carlo ----------------------------------------------
MC_SRCS = Tests/TestMonteCarlo.cpp \
			RandomGenerators/RandomGenerator.cpp \
			RandomGenerators/UniformGenerators/EcuyerCombined.cpp \
			RandomGenerators/UniformGenerators/LinearCongruential.cpp \
			RandomGenerators/UniformGenerators/PseudoGenerator.cpp \
			RandomGenerators/UniformGenerators/UniformGenerator.cpp \
			RandomGenerators/ContinuousGenerators/ContinuousGenerator.cpp \
			RandomGenerators/ContinuousGenerators/Normal.cpp \
			Processes/SinglePath.cpp \
			Processes/RandomProcess.cpp \
			Processes/BSEuler1D.cpp \
			Processes/BlackScholes1D.cpp \
			Processes/BSEulerND.cpp \
			Processes/BlackScholesND.cpp \
			Processes/BSEulerNDAnti.cpp \
			Processes/Underlyings/Underlying.cpp \
			Processes/Underlyings/Basket.cpp \
			Pricers/MonteCarlo/MonteCarlo.cpp \
			Payoffs/Payoff.cpp \
			Payoffs/EuropeanCall.cpp \
			Pricers/Pricer.cpp \
			Utils/Input.cpp \
			Utils/Output.cpp \
			Utils/Matrix.cpp

MC_OBJS = $(MC_SRCS:.cpp=.o)

test_mc: $(MC_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(MC_OBJS)

# Pattern rule to compile .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


# Cleaning Rules
clean_random:
	rm -f $(GEN_OBJS) test_random

clean_pde:
	rm -f $(PDE_OBJS) test_pde

clean_proc:
	rm -f $(PROC_OBJS) test_proc

clean_ls:
	rm -f $(LS_OBJS) test_ls

clean_mc:
	rm -f $(MC_OBJS) test_mc

clean: clean_random clean_pde clean_proc clean_ls clean_mc
