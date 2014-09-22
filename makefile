CXXFLAGS=-Wall -Wextra -Wfatal-errors -pedantic -std=c++11 -O3
all:
	$(CXX) $(CXXFLAGS) -o test test.cpp
	$(CXX) $(CXXFLAGS) -S -masm=intel -o example.s example.cpp
