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
#include <sdr_simulator/util/Stimulus.hpp>
#include <sdr_simulator/util/FileRecorder.hpp>
#include <sdr_simulator/util/GaussianNoiseGenerator.hpp>

using namespace std;

int main()
{
   const double TIME_RESOLUTION = 1.0;
   const double CLOCK_PERIOD = 15.68;
   const double TOTAL_SIMULATION_TIME = CLOCK_PERIOD*5e5;
   const unsigned int RESET_TIME = 20;

   const double MEAN = 0.0;
   const double VARIANCE = 1.0;
   const double AMPLITUDE = 0.25;

   const string RECORDER_FILE_NAME = "output.dat";

   sc_signal< data_input_type > dut_input_signal;
   sc_signal< data_output_type > dut_output_signal;
   sc_signal< data_input_type > decimate_signal;

   decimate_signal = 8;

   // set time parameters
   sc_set_time_resolution( TIME_RESOLUTION , SC_NS );
   sc_time simulation_time( TOTAL_SIMULATION_TIME,SC_NS );
   sc_time clock_time(CLOCK_PERIOD,SC_NS);

   Stimulus<reset_type> stimulus("stimulus", clock_time, RESET_TIME );

   // gaussian noise source
   GaussianNoiseGenerator<data_output_type, reset_type > 
      gaussianNoiseGen( "noise_gen", MEAN, VARIANCE, AMPLITUDE );
   gaussianNoiseGen.output( dut_input_signal );
   gaussianNoiseGen.reset( stimulus.reset );
   gaussianNoiseGen.clock( stimulus.clock );

   // DUT
   Cic< data_input_type , data_output_type > cic( "cic" );
   cic.clock( stimulus.clock );
   cic.reset( stimulus.reset );
   cic.input( dut_input_signal );
   cic.output( dut_output_signal );
   cic.decimation( decimate_signal );

   // record output to file
   FileRecorder<data_output_type, reset_type> record( "record", RECORDER_FILE_NAME );
   record.input( dut_output_signal );
   record.reset( stimulus.reset );
   record.clock( cic.div_clock );

   // begin simulation
   sc_start( simulation_time );
}
