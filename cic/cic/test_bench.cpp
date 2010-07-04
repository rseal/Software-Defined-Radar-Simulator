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
#include "Cic.hpp"
#include "test_bench.hpp"
#include <sdr_simulator/util/FileRecorder.hpp>
#include <sdr_simulator/util/GaussianNoiseStimulus.hpp>

using namespace std;
using boost::lexical_cast;

int main()
{
   const double TIME_RESOLUTION = 1.0;
   const double TOTAL_SIMULATION_TIME = 500000.0;
   const double CLOCK_PERIOD = 2.0;
   const char* FILE_NAME = "output.dat";
   const double MEAN = 0.0;
   const double VARIANCE = 1.0;
   const double AMPLITUDE = 0.25;

   sc_signal< testbench::bit_type > reset_signal;
   sc_signal< testbench::data_input_type > input_signal;
   sc_signal< testbench::data_output_type > output_signal;
   sc_signal< bool > clk_signal;
   sc_signal< testbench::data_input_type > decimate_signal;

   decimate_signal = testbench::DECIMATION;

   // set time parameters
   sc_set_time_resolution( TIME_RESOLUTION , SC_NS );
   sc_time simulation_time(TOTAL_SIMULATION_TIME,SC_NS);
   sc_time clock_time(CLOCK_PERIOD,SC_NS);

   // gaussian noise source
   GaussianNoiseStimulus<testbench::data_input_type, testbench::INPUT_WIDTH> 
      stimulus( "stimulus", MEAN, VARIANCE, AMPLITUDE );
   stimulus.reset( reset_signal );
   stimulus.output( output_signal );

   // record output to file
   FileRecorder<testbench::data_input_type> record( "record", FILE_NAME );
   record.input( input_signal );
   record.clock( stimulus.clock );

   // DUT
   Cic< testbench::INPUT_WIDTH, testbench::OUTPUT_WIDTH > 
      cic( "cic" );
   cic.clock( stimulus.clock );
   cic.reset( reset_signal );
   cic.input( output_signal );
   cic.output( input_signal );
   cic.decimation( decimate_signal );

   // begin simulation
   sc_start( simulation_time );
}
