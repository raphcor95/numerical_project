# -*- Makefile -*-

# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall \
			   -I./Utils \
               -I./RandomGenerators \
               -I./RandomGenerators/UniformGenerators \
			   -I./Pricers/PDE

all: test_random test_pde


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
		   Utils/Output.cpp

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



# Pattern rule to compile .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean_random:
	rm -f $(GEN_OBJS) test_random

clean_pde:
	rm -f $(PDE_OBJS) test_pde
