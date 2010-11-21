Software Defined Radar Simulator Notes
Date:    June 26, 2010
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
2. Ruby ( required for install-header script).
3. Python ( only required for copyright installation script ).
4. Latest version of Gnu Compiler Collection (GCC) software.
5. Latest version of Boost libraries ( www.boost.org ).
6. Scons build tool.

Installation Notes:

At this point, the SDRS software is for development use only. Two steps are
involved to build:
1. As the root user, run the "install-headers" script in the scripts folder.
2. As a standard user, run scons in subdirectories with an SConstruct file.

There are currently several functioning blocks complete. However, the effort
to move these binary tests to a more unified structure ( i.e. boost-test ) has
yet to be completed. As development progresses, there are a number of TODO
items that will take place:

1. Create more generalized classes to assist in test bench tests as well as
unit tests.
2. For each block, unit tests should be mandatory via boost test, and all
tests should be linked to bjam's build system for automated testing.
3. The RX receive chain, when complete, should provide an XML-based parameter
tool to enable easy implementation of graphical tools at a later date.

Code submission pre-commit:
1. Run the dist-clean script in the script folder to perform proper clean-up.

