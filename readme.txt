Software Defined Radar Simulator Notes
Date:    June 26, 2010
Version: 0.99
Author:  Ryan Seal

Introduction:
The Software Defined Radar Simulator software is designed to provide an
accurate simulation of commonly-used SDR blocks. These models provide both
run-time and compile-time adjustable parameters. 

TODO: Provide more information here.

Dependencies:
1. The latest version of the systemc software.
2. Ruby ( required for install-header script).
3. Python ( only required for copyright installation script ).
4. Latest version of Gnu Compiler Collection (GCC) software.
5. Latest version of Boost libraries ( www.boost.org ).

Installation Notes:

At this point, the SDRS software is for development use only. By running the
install-headers software ( as ROOT ), a directory and soft links will be added
to the /usr/local/include/sdr_simulator directory, pointing back to the files
located in the local include/ folder of this project. From there, the user can
build files in each directory using the usual method.

TODO: Add bjam build scripts and project files to each directory.

