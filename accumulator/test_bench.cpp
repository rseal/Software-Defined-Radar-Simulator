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

#include <tr1/math.h>
#include <sdr_simulator/PhaseAccumulator.hpp>
#include <sdr_simulator/util/FileRecorder.hpp>
#include <sdr_simulator/util/Stimulus.hpp>
#include <sdr_simulator/sc_timer.hpp>
#include <sdr_simulator/Types.hpp>

#include "TruncatedRecorder.hpp"

#include <iostream>
#include <boost/math/constants/constants.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost::math;
using namespace boost;

int sc_main(int argc, char* argv[]){

  const int BIT_WIDTH = 32;
  const double SAMPLE_RATE = 64e6;
  const double FREQUENCY = 31.0e6;
  const double RESET_HOLD_TIME = 5;
  const string RECORDER_FILE_NAME = "output.dat";
  const string TRUNC_RECORDER_FILE_NAME = "trunc_output.dat";
  const double CLOCK_PERIOD = 15.68; //NS

  // define signals and data types 
  typedef sc_int< BIT_WIDTH > data_output_type;
  sdr_types::reset_signal reset_signal;
  sc_signal< data_output_type > out_signal;
  sc_signal< data_output_type > null_signal;

  // define system clock 
  sc_clock clock("clk",sc_time(CLOCK_PERIOD,SC_NS));

  // determine phase step size from desired frequency
  int step_size = static_cast<int>( tr1::pow(2.0,BIT_WIDTH*1.0)*FREQUENCY/SAMPLE_RATE);

  // display settings
  cout 
     << "\nsample rate = " << SAMPLE_RATE 
     << "\n  frequency = " << FREQUENCY 
     << "\n  bit width = " << BIT_WIDTH 
     << "\n  step size = " << step_size 
     << "\n\n";
  
  // define testbench stimulus
  Stimulus< data_output_type > stimulus( "stimulus", RESET_HOLD_TIME, clock);
  stimulus.output( null_signal );

  // DUT
  PhaseAccumulator<BIT_WIDTH> accumulator("accumulator", step_size );
  accumulator.clock( stimulus.clock );
  accumulator.out(out_signal);
  accumulator.reset( stimulus.reset );

  // record output
  FileRecorder< data_output_type > record( "recorder", RECORDER_FILE_NAME );
  record.clock( stimulus.clock );
  record.input( out_signal );

  TruncatedRecorder< data_output_type > t_record( "trunc_recorder", TRUNC_RECORDER_FILE_NAME );
  t_record.clock( stimulus.clock );
  t_record.input( out_signal );


  //run simulations for 22 nsec
  sc_start(sc_time(5000, SC_NS));

  return 0;
}
