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
#include <sdr_simulator/Cordic.hpp>
#include "test_bench.hpp"
#include "stimulus.hpp"

int sc_main( int argc, char* argv[])
{
   
   // create constants
   const unsigned int NUM_STAGES = 12;
   const double TIME_RESOLUTION = 100.0;
   const double CLOCK_PERIOD = 15.68;
   const double TOTAL_SIMULATION_TIME = 1e6*CLOCK_PERIOD;

   // set time parameters
   sc_set_time_resolution( TIME_RESOLUTION , SC_PS );
   sc_time simulation_time( TOTAL_SIMULATION_TIME,SC_NS );
   sc_time clock_time( CLOCK_PERIOD,SC_NS );

   // generate clock 
   sc_clock clock("clk",clock_time);

   // create connecting signals from stimulus-->dut
   sc_signal<test_bench::data_type> x_stim,y_stim;
   sc_signal<test_bench::z_type> z_stim;
   sc_signal<test_bench::data_type> x_out,y_out;
   sc_signal<test_bench::z_type> z_out;
   sc_signal<test_bench::bit_type> reset_stim;
   sc_signal<test_bench::z_type> theta_stim;

   // create stimulus signal object
   Stimulus stim("stimulus", TOTAL_SIMULATION_TIME);
   stim.reset(reset_stim);
   stim.clock(clock);
   stim.x_in(x_out);
   stim.y_in(y_out);
   stim.z_in(z_out);
   stim.x_out(x_stim);
   stim.y_out(y_stim);
   stim.z_out(z_stim);

   // create cordic_stage object ( DUT )
   Cordic< test_bench::DATA_WIDTH, test_bench::Z_WIDTH > dut("cordic", NUM_STAGES);
   dut.xin(x_stim);
   dut.yin(y_stim);
   dut.zin(z_stim);
   dut.reset(reset_stim);
   dut.xout(x_out);
   dut.yout(y_out);
   dut.zout(z_out);
   dut.clock(clock);

   // begin simulation
   sc_start( simulation_time );

   return 0;
}
