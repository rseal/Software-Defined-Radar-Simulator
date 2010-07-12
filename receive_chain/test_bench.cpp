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
#include <sdr_simulator/ReceiveChain.hpp>
#include <sdr_simulator/util/GaussianNoiseStimulus.hpp>
#include <sdr_simulator/util/SinusoidStimulus.hpp>
#include <sdr_simulator/util/FileRecorder.hpp>
#include <systemc.h>
#include <iostream>


int main (void) {

   const double TIME_RESOLUTION = 100.0;
   const double TOTAL_SIMULATION_TIME = 500000.0;
   const double CLOCK_PERIOD = 15.68;
   const std::string X_RECORDER_FILE_NAME = "inphase.dat";
   const std::string Y_RECORDER_FILE_NAME = "quadrature.dat";
   const unsigned int RESET_TIME = 20;
   const double AMPLITUDE = 0.75;
   const double FREQUENCY = 20.0e6;
   const double SAMPLERATE = 64e6;
   const double NORMALIZED_FREQUENCY = FREQUENCY/SAMPLERATE;
   const unsigned int DECIMATION = 8;

   const unsigned int INPUT_WIDTH = 16;
   const unsigned int OUTPUT_WIDTH = 16;

   typedef sc_int< INPUT_WIDTH > data_input_type;
   typedef sc_int< OUTPUT_WIDTH > data_output_type;
   typedef sc_int< INPUT_WIDTH > decimation_type;
   typedef bool bit_type;

   sc_signal < data_input_type > input_signal;
   sc_signal < data_output_type > x_output_signal;
   sc_signal < data_output_type > y_output_signal;
   sc_signal < decimation_type > decimation_signal;
   sc_signal < bit_type > reset;

   sc_set_time_resolution( TIME_RESOLUTION , SC_PS );
   sc_time simulation_time(TOTAL_SIMULATION_TIME,SC_NS);
   sc_time clock_time(CLOCK_PERIOD,SC_NS);
   sc_clock stimulus_clock( "clock", clock_time);

   decimation_signal.write( DECIMATION );

   // sinusoidal noise test bench stimulus
   SinusoidStimulus< data_output_type, OUTPUT_WIDTH> stimulus( 
         "stimulus", 
         RESET_TIME,
         stimulus_clock,
         NORMALIZED_FREQUENCY,
         AMPLITUDE
         );
   stimulus.output( input_signal );
   
   // test bench data recorder
   FileRecorder< data_output_type > xRecorder( "xRecorder", X_RECORDER_FILE_NAME );
   xRecorder.input( x_output_signal );
   xRecorder.clock( stimulus.clock );

   // test bench data recorder
   FileRecorder< data_output_type > yRecorder( "yRecorder", Y_RECORDER_FILE_NAME );
   yRecorder.input( y_output_signal );
   yRecorder.clock( stimulus.clock );

   // DUT
   ReceiveChain< INPUT_WIDTH, OUTPUT_WIDTH> receiveChain( "rx_chain");
   receiveChain.input( input_signal );
   receiveChain.x_output( x_output_signal );
   receiveChain.y_output( y_output_signal );
   receiveChain.reset( stimulus.reset );
   receiveChain.clock( stimulus.clock );
   receiveChain.decimation( decimation_signal );

   // begin simulation
   sc_start( simulation_time );

   return 0;

}
