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
#include <boost/lexical_cast.hpp>
#include <vector>

#include <sdr_simulator/filter/cic/CicDifferentiator.hpp>
#include <sdr_simulator/util/FileRecorder.hpp>
#include <sdr_simulator/util/Stimulus.hpp>
#include <sdr_simulator/util/SignalGenerator.hpp>

#include "test_bench.hpp"

using namespace std;
using boost::lexical_cast;

int sc_main(int argc, char* argv[] )
{
   const double TIME_RESOLUTION = 1.0;
   const double TOTAL_SIMULATION_TIME = 5e5;
   const int RESET_TIME = 10;
   const double CLOCK_PERIOD = 2.0;
   const std::string OUTPUT_FILE_NAME = "data.dat";

   sc_signal< INPUT_TYPE > input_signal;
   sc_signal< OUTPUT_TYPE > output_signal;

   // set time parameters
   sc_set_time_resolution( TIME_RESOLUTION , SC_NS );
   sc_time simulation_time(TOTAL_SIMULATION_TIME,SC_NS);
   sc_time clock_time(CLOCK_PERIOD,SC_NS);

   // clock and reset stimulus
   Stimulus< RESET_TYPE >  stimulus( "stimulus", clock_time, RESET_TIME );

   SignalGenerator< INPUT_TYPE, RESET_TYPE > sigGen("sig_gen", 1024);
   sigGen.output( input_signal );
   sigGen.clock( stimulus.clock );
   sigGen.reset( stimulus.reset );

   // DUT 
   CicDifferentiator< INPUT_WIDTH, OUTPUT_WIDTH > 
      cic_differentiator( "cic_differentiator" );
   cic_differentiator.clock( stimulus.clock );
   cic_differentiator.reset( stimulus.reset );
   cic_differentiator.input( input_signal );
   cic_differentiator.output( output_signal );

   // record dut output data to file.
   FileRecorder< INPUT_TYPE, RESET_TYPE>  record( "record", OUTPUT_FILE_NAME );
   record.clock( stimulus.clock );
   record.reset( stimulus.reset );
   record.input( output_signal );

   // record dut output data to file.
   FileRecorder< INPUT_TYPE, RESET_TYPE>  inputRecord( "input", "input.dat" );
   inputRecord.clock( stimulus.clock );
   inputRecord.reset( stimulus.reset );
   inputRecord.input( input_signal );

   // begin simulation
   sc_start( simulation_time );

   return 0;
}
