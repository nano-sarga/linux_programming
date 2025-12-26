CXX      = g++
CXXFLAGS = -Wall -Wextra -O2
TARGETS  = first second

all: $(TARGETS)

shared_array.o: shared_array.cpp shared_array.h
	$(CXX) $(CXXFLAGS) -c shared_array.cpp

first: first.cpp shared_array.o
	$(CXX) $(CXXFLAGS) first.cpp shared_array.o -o first -pthread

second: second.cpp shared_array.o
	$(CXX) $(CXXFLAGS) second.cpp shared_array.o -o second -pthread

clean:
	rm -f *.o first second
