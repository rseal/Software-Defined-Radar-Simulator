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
//#include <sdr_simulator/CicIntegrator.hpp>

#include <boost/lexical_cast.hpp>
#include <vector>
#include <sdr_simulator/util/GaussianNoiseGenerator.hpp>
#include <sdr_simulator/util/FileRecorder.hpp>
#include <sdr_simulator/util/Stimulus.hpp>

#include "test_bench.hpp"

using namespace std;
using boost::lexical_cast;

int sc_main(int argc, char* argv[])
{
   // constants
   const double TIME_RESOLUTION       = 1.0;
   const double TOTAL_SIMULATION_TIME = 50000.0;
   const double CLOCK_PERIOD          = 2.0;
   const double RESET_TIME            = 10.0;
   const float MEAN                   = 0.0;
   const float VARIANCE               = 1.0;
   const float AMPLITUDE              = 0.125;
   const char* RECORD_FILE_NAME     = "output.dat";

   // signal definition
   sc_signal< testbench::OUTPUT_TYPE > dut_output_signal;

   // set time parameters
   sc_set_time_resolution( TIME_RESOLUTION , SC_NS );
   sc_time simulation_time(TOTAL_SIMULATION_TIME,SC_NS);
   sc_time clock_time(CLOCK_PERIOD,SC_NS);

   // signal stimulus
   Stimulus<testbench::RESET_TYPE> stimulus( "stimulus", clock_time, RESET_TIME );

   // test signal generator
   GaussianNoiseGenerator< testbench::OUTPUT_TYPE, testbench::RESET_TYPE > 
      tsg( "TestSignalGenerator", MEAN, VARIANCE, AMPLITUDE );
   tsg.reset( stimulus.reset );
   tsg.clock( stimulus.clock );
   tsg.output( dut_output_signal );

   // output recorder
   FileRecorder< testbench::OUTPUT_TYPE, testbench::RESET_TYPE> record( "record", RECORD_FILE_NAME );
   record.input( dut_output_signal );
   record.reset( stimulus.reset );
   record.clock( stimulus.clock );

   // begin simulation
   sc_start( simulation_time );

   return 0;
}
