all: test

NAME=phase_accumulator
INCLUDE=/usr/local/include/sdr_simulator

FLAGS= -march=native -msse3 -pipe

.PHONY: test

test: test_bench.cpp $(INCLUDE)/PhaseAccumulator.hpp 
	g++ $(FLAGS) -o test test_bench.cpp -lsystemc

clean: 
	rm -rf test *~ *.dat
	
	
