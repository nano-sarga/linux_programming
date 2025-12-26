CXX = g++
CXXFLAGS = -std=c++17 -O2 -pthread

all: demo

demo: parallel_scheduler.o demo.o
	$(CXX) $(CXXFLAGS) $^ -o demo

parallel_scheduler.o: parallel_scheduler.cpp parallel_scheduler.h
	$(CXX) $(CXXFLAGS) -c parallel_scheduler.cpp

demo.o: demo.cpp
	$(CXX) $(CXXFLAGS) -c demo.cpp

clean:
	rm -f *.o demo

