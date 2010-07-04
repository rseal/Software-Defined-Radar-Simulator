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
#include <systemc.h>

#include <sdr_simulator/PhaseAccumulator.hpp>
#include <sdr_simulator/sc_timer.hpp>

#include <iostream>
#include <boost/math/constants/constants.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost::math;
using namespace boost;

int sc_main(int argc, char* argv[]){

typedef sc_uint<1> bit_type;

  // setup constants
  const string DATE = "May 07, 2010";
  const string AUTHOR_NAME = "Ryan Seal";
  const string VERSION = "v0.99";
  const string MODULE_NAME = "PHASE ACCUMULATOR";
  const string MODULE_DISPLAY_STRING = 
     MODULE_NAME  + " TEST BENCH " + VERSION + " " + DATE + 
     "AUTHOR: " + AUTHOR_NAME; 
  const int BIT_WIDTH = 32;
  const double SAMPLE_RATE = 64e6;
  const double FREQUENCY = 14.2e6;
  const double RESET_HOLD_TIME = 5;
  const string VCD_FILE = "test";

  // setup master clock
  sc_set_time_resolution(100,SC_PS);
  sc_clock clock("clk",sc_time(15.68,SC_NS));

  // determine phase step size from desired frequency
  int step_size = static_cast<int>(pow(2.0,BIT_WIDTH)*FREQUENCY/SAMPLE_RATE);

  cout << "\n" << MODULE_DISPLAY_STRING << "\n";
  // display settings
  cout 
     << "\nsample rate = " << SAMPLE_RATE 
     << "\n  frequency = " << FREQUENCY 
     << "\n  bit width = " << BIT_WIDTH 
     << "\n  step size = " << step_size 
     << "\n\n";

  // create vcd trace file
  sc_trace_file* trace_file = sc_create_vcd_trace_file(VCD_FILE.c_str());
  sc_signal<bit_type> reset_signal;
  sc_signal<sc_uint<BIT_WIDTH> > out_signal;

  // create a constant low
  sc_signal<bit_type> low_signal;
  low_signal.write(0);

  sc_timer rst ("reset", RESET_HOLD_TIME);
  rst.clk(clock);
  rst.reset(low_signal);
  rst.time_out(reset_signal);

  PhaseAccumulator<BIT_WIDTH> accumulator("accumulator", step_size );
  accumulator.clock(clock);
  accumulator.out(out_signal);
  accumulator.reset(reset_signal);
  
  sc_trace(trace_file, clock, "clock");
  sc_trace(trace_file, rst.time_out, "time_out");
  sc_trace(trace_file, accumulator.out, "phase");

  sc_start(SC_ZERO_TIME);

  //run simulations for 22 nsec
  sc_start(sc_time(5000, SC_NS));

  // close vcd file
  sc_close_vcd_trace_file(trace_file);
  
  return 0;
}
