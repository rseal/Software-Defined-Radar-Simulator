Software Defined Radar Simulator Notes
Date:    July 28, 2012
Version: 0.99
Author:  Ryan Seal

Introduction:
The Software Defined Radar Simulator software is designed to provide an
accurate simulation of commonly-used SDR blocks. These models provide both
run-time and compile-time adjustable parameters. Currently these blocks
include:

1. Phase Accumulator
2. CORDIC down-converter
3. CIC Filter
4. FIR Filter
5. Stimulus class to assist in test bench development
6. Signal Generator classes to assist in testing.
7. Recorder class to collect test output data.

Dependencies:
1. The latest version of the systemc software.
2. Latest version of Gnu Compiler Collection (GCC) software.
3. Latest version of Boost libraries ( www.boost.org ).
4. Waf build tool.
5. Latest version of yaml-cpp library.

Installation Notes:

At this point, the SDRS software is for development use only. Two steps are
involved to build:
1. As the root user, run "waf install_headers" script in the scripts folder.
2. From the project root, run "git submodule init" to pull in the tinyxml dependency ( see deps folder ).
2. As a standard user, run "waf config build" in subdirectories containing a wscript file.

Code submission pre-commit:
1. Run the cleanup.sh script in the script folder to perform proper clean-up.

