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
#include "Cic.hpp"
#include "test_bench.hpp"
#include <sdr_simulator/util/FileRecorder.hpp>
#include <sdr_simulator/util/GaussianNoiseGenerator.hpp>

using namespace std;

int main()
{
   const double TIME_RESOLUTION = 1.0;
   const double TOTAL_SIMULATION_TIME = 500000.0;
   const double CLOCK_PERIOD = 2.0;
   const unsigned int RESET_TIME = 20;

   const double MEAN = 0.0;
   const double VARIANCE = 1.0;
   const double AMPLITUDE = 0.25;
   const string RECORDER_FILE_NAME = "output.dat";

   sc_signal< reset_type> reset_signal;
   sc_signal< data_input_type > dut_input_signal;
   sc_signal< data_output_type > dut_output_signal;
   sc_signal< bool > clk_signal;
   sc_signal< data_input_type > decimate_signal;

   decimate_signal = 8;

   // set time parameters
   sc_set_time_resolution( TIME_RESOLUTION , SC_NS );
   sc_time simulation_time(TOTAL_SIMULATION_TIME,SC_NS);
   sc_time clock_time(CLOCK_PERIOD,SC_NS);
   sc_clock stimulus_clock( "clock", clock_time);

   // gaussian noise source
   GaussianNoiseGenerator<data_output_type, reset_type > 
      gaussianNoiseGen( "stimulus", MEAN, VARIANCE, AMPLITUDE );
   gaussianNoiseGen.output( dut_input_signal );
   gaussianNoiseGen.reset( reset_signal );
   gaussianNoiseGen.clock( stimulus_clock );

   // record output to file
   FileRecorder<data_output_type, reset_type> record( "record", RECORDER_FILE_NAME );
   record.input( dut_output_signal );
   record.reset( reset_signal );
   record.clock( stimulus_clock );

   // DUT
   Cic< INPUT_WIDTH, OUTPUT_WIDTH > 
      cic( "cic" );
   cic.clock( stimulus_clock );
   cic.reset( reset_signal );
   cic.input( dut_input_signal );
   cic.output( dut_output_signal );
   cic.decimation( decimate_signal );

   // begin simulation
   sc_start( simulation_time );
}
