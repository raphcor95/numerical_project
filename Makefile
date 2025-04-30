# -*- Makefile -*-

# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall \
               -I./RandomGenerators \
               -I./RandomGenerators/UniformGenerators

# ------------------------------
# Tests
# ------------------------------

# Generators
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
           RandomGenerators/ContinuousGenerators/Exponential.cpp

GEN_OBJS = $(GEN_SRCS:.cpp=.o)

test_random: $(GEN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(GEN_OBJS)

# Pattern rule to compile .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Utilities
all: test_random

clean:
	rm -f $(GEN_OBJS) test_random
