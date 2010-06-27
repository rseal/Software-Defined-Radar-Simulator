// Copyright (c) 2010 Ryan Seal <rlseal -at- gmail.com>
//
// This file is part of Software Defined Radar Simulator (SDRS) Software.
//
// SDRS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//  
// SDRS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SDRS.  If not, see <http://www.gnu.org/licenses/>.
#include <sdr_simulator/CordicShiftAdder.hpp>
#include "stimulus.hpp"

int sc_main( int argc, char* argv[])
{
   // create constants
   const double TIME_RESOLUTION = 1.0;
   const double TOTAL_SIMULATION_TIME = 500.0;
   const double CLOCK_PERIOD = 2.0;
   const unsigned int INPUT_WIDTH = 18;
   const unsigned int OUTPUT_WIDTH = 18;
   
   // typedef signal types
   typedef sc_int<INPUT_WIDTH> input_type;
   typedef sc_int<OUTPUT_WIDTH> output_type;
   typedef sc_uint<1> bit_type;

   // set time parameters
   sc_set_time_resolution( TIME_RESOLUTION , SC_NS );
   sc_time simulation_time(TOTAL_SIMULATION_TIME,SC_NS);
   sc_time clock_time(CLOCK_PERIOD,SC_NS);

   // generate clock 
   sc_clock clock("clock",clock_time);

   // create connecting signals from stimulus-->dut
   sc_signal<input_type> x,y;
   sc_signal<output_type> out;
   sc_signal<bit_type> reset;
   sc_signal<bit_type> sign;

   // create stimulus signal object
   Stimulus<INPUT_WIDTH> stim("stimulus", TOTAL_SIMULATION_TIME);
   stim.clock(clock);
   stim.reset(reset);
   stim.x(x);
   stim.y(y);
   stim.sign(sign);
   stim.out(out);

   // create dut
   CordicShiftAdder<INPUT_WIDTH, OUTPUT_WIDTH> csa("cordic_shift_adder", stim.SHIFT);
   csa.clock(clock);
   csa.reset(reset);
   csa.in_a(x);
   csa.in_b(y);
   csa.out(out);
   csa.sign(sign);

   // begin simulation
   sc_start( simulation_time );
}
