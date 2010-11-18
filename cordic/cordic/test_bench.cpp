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
#include <sdr_simulator/util/SinusoidGenerator.hpp>
#include <sdr_simulator/PhaseAccumulator.hpp>
#include <sdr_simulator/util/FileRecorder.hpp>

#include "test_bench.hpp"
#include "Accumulator.hpp"

int sc_main( int argc, char* argv[])
{
   
   // define clock period and simulation resolution.
   const double TIME_RESOLUTION = 100.0;
   const double CLOCK_PERIOD = 1.0e9 / SAMPLE_RATE;

   // simulation time
   const double TOTAL_SIMULATION_TIME = 1e6*CLOCK_PERIOD;

   // output file names
   const string X_DATA_FILE_NAME = "xout.dat";
   const string Y_DATA_FILE_NAME = "yout.dat";
   const string Z_DATA_FILE_NAME = "zout.dat";
   const string INPUT_SIGNAL_FILE_NAME = "inputSignal.dat";

   // set time parameters
   sc_set_time_resolution( TIME_RESOLUTION , SC_PS );
   sc_time simulation_time( TOTAL_SIMULATION_TIME,SC_NS );
   sc_time clock_time( CLOCK_PERIOD, SC_NS );

   // generate clock 
   sc_clock clock("clk",clock_time);

   // interface signals
   sc_signal<reset_type> reset_signal;
   sc_signal<data_output_type> x_data_signal;
   sc_signal<data_output_type> y_data_signal;
   sc_signal<z_output_type> phase_data_signal;
   sc_signal<data_output_type> x_data_output_signal;
   sc_signal<data_output_type> y_data_output_signal;
   sc_signal<data_output_type> z_data_output_signal;

   // the y-input to the CORDIC is constant. This will 
   // peform down-conversion and provide a complex output.
   y_data_signal.write(0);

   // generate a sinusoid to feed into the x-input for testing.
   SinusoidGenerator<data_output_type, reset_type > 
      sineGenerator( "sine_generator", NORMALIZED_FREQUENCY, ADC_WIDTH, 0.9 );
   sineGenerator.reset( reset_signal );
   sineGenerator.clock( clock );
   sineGenerator.output( x_data_signal );

   // Store the input signal into a file.
   FileRecorder< data_output_type, reset_type > inputSignalRecorder( 
         "input_signal_data_recorder", INPUT_SIGNAL_FILE_NAME );
   inputSignalRecorder.clock( clock );
   inputSignalRecorder.reset( reset_signal );
   inputSignalRecorder.input( x_data_signal );

   // Create an accumulator object to feed into the theta ( phase ) input.
   PhaseAccumulator< z_output_type, reset_type> 
      phaseAccumulator( "phase_accumulator", ACCUMULATOR_STEP_SIZE );
   phaseAccumulator.clock( clock );
   phaseAccumulator.reset( reset_signal );
   phaseAccumulator.out( phase_data_signal );

   // Store the x data output into a file.
   FileRecorder< data_output_type, reset_type > xDataRecorder( 
         "x_data_recorder", X_DATA_FILE_NAME );
   xDataRecorder.clock( clock );
   xDataRecorder.reset( reset_signal );
   xDataRecorder.input( x_data_output_signal );

   // Store the y data output into a file.
   FileRecorder< data_output_type, reset_type > yDataRecorder( 
         "y_data_recorder", Y_DATA_FILE_NAME );
   yDataRecorder.clock( clock );
   yDataRecorder.reset( reset_signal );
   yDataRecorder.input( y_data_output_signal );

   // Store the z data output into a file.
   FileRecorder< data_output_type, reset_type > zDataRecorder( 
         "z_data_recorder", Z_DATA_FILE_NAME );
   zDataRecorder.clock( clock );
   zDataRecorder.reset( reset_signal );
   zDataRecorder.input( z_data_output_signal );

   // create cordic_stage object ( DUT )
   Cordic< DATA_WIDTH, Z_WIDTH > dut("cordic", NUM_STAGES);
   dut.xin(x_data_signal);
   dut.yin(y_data_signal);
   dut.zin( phase_data_signal );
   dut.reset(reset_signal);
   dut.xout( x_data_output_signal );
   dut.yout(y_data_output_signal );
   dut.zout(z_data_output_signal );
   dut.clock(clock);

   // begin simulation
   sc_start( simulation_time );

   return 0;
}
