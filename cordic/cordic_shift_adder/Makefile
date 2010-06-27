all: test

NAME=cordic_shift_adder

INCLUDE=/usr/local/include/sdr_simulator
HEADERS=$(shell ls -1 $(INCLUDE)/*.hpp)

FLAGS= -march=native -msse3 -pipe

.PHONY: test

test: test_bench.cpp $(HEADERS)
	g++ $(FLAGS) -o test test_bench.cpp -lsystemc

clean: 
	rm -rf test *~ *.dat
	
	
