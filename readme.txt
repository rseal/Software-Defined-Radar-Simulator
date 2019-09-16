Software Defined Radar Simulator Notes
Date:    July 28, 2013
Version: 0.99
Author:  Ryan Seal

Introduction:
The Software Defined Radar Simulator software is designed to provide an
accurate simulation of commonly-used SDR blocks. These models provide both
run-time and compile-time adjustable parameters. Currently these blocks
include:

1. Phase Accumulator
2. CORDIC down-converter
3. CIC Filter (includes optional bit-pruning)
4. FIR Filter
5. Stimulus class to assist in test bench development
6. Signal Generator classes to assist in testing.
7. Recorder class to collect test output data.

Dependencies:
1. SystemC version 2.3 software.
2. Latest version of GCC.
3. Latest version of Boost libraries ( www.boost.org ).
4. Waf build tool.
5. Yaml-cpp version 0.5.1 software. 

Installation Notes:

1. From the root folder, cd to scripts and execute: 
   a. waf distclean
   b. waf configure
   c. sudo waf install_headers

2. From the root folder, build the sdr library by executing : 
   a. waf configure
   b. waf build

3. Go to the <project>/receivers/usrp_v1 folder.
   a. The CIC filter parameters can be modified in sdr.yml (see cic section).
   b. waf configure
   c. waf build --gc
   d. waf build 
   e. Go to the "build" folder and execute "test_rx_channel" to run sample data through the receiver.
   f. Output data will be captured as ASCII data in "*.dat" files. 
   g. Runtime parameters can be edited in stimulus.yml
   h. The input stimulus can be replaced in test_bench.cpp (see include/input/xxx for list of inputs).

