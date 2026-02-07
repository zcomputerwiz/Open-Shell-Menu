CXX = g++
CXXFLAGS = -std=c++11 -Wall -I.

all: behavior_tests

behavior_tests: StartMenuBehaviorTests.cpp StartMenuTestHarness.h
	$(CXX) $(CXXFLAGS) StartMenuBehaviorTests.cpp -o behavior_tests

test: behavior_tests
	./behavior_tests

clean:
	rm -f behavior_tests
