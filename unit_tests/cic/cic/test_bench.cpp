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
#include <sdr_simulator/util/SinusoidGenerator.hpp>
#include <sdr_simulator/util/PulseGenerator.hpp>
#include <sdr_simulator/util/NoisySinusoidGenerator.hpp>

using namespace std;

int sc_main(int argc, char* argv[])
{
   const double TIME_RESOLUTION       = 100.0;
   const double CLOCK_PERIOD          = 15.600;
   const double TOTAL_SIMULATION_TIME = CLOCK_PERIOD*1e5;
   const unsigned int RESET_TIME      = 10;

   const string RECORDER_FILE_NAME = "output.dat";

   sc_signal< cic::INPUT_TYPE > dut_input_signal;
   sc_signal< cic::OUTPUT_TYPE > dut_output_signal;
   sc_signal< cic::INPUT_TYPE > decimate_signal;

   double pulse_width = 400.0e-6;
   double ipp_width   = 40000.0e-6;
   double fsignal     = 1e6;
   double sample_rate = 64e6;


   int decimation = 16;
   decimate_signal = decimation;

   // set time parameters
   sc_set_time_resolution( TIME_RESOLUTION , SC_PS );
   sc_time simulation_time( TOTAL_SIMULATION_TIME,SC_NS );
   sc_time clock_time(CLOCK_PERIOD,SC_NS);

   Stimulus< cic::RESET_TYPE > stimulus("stimulus", clock_time, RESET_TIME );

   // gaussian noise source
   //GaussianNoiseGenerator< cic::OUTPUT_TYPE, cic::RESET_TYPE > 
   // gaussianNoiseGen( "noise_gen", 0.0, 1.0, 0.25);
   //gaussianNoiseGen.output( dut_input_signal );
   //gaussianNoiseGen.reset( stimulus.reset );
   //gaussianNoiseGen.clock( stimulus.clock );

   //NoisySinusoidGenerator< cic::OUTPUT_TYPE, cic::RESET_TYPE> 
   //signal_generator( "sig_gen", normalized_frequency, 12, 0.0, 1.0, 1.0, 0.0000001 );
   //signal_generator.output( dut_input_signal );
   //signal_generator.reset( stimulus.reset );
   //signal_generator.clock( stimulus.clock );

   //PulseGenerator( 
         //const sc_module_name& nm, 
         //const double pw, 
         //const double pri, 
         //const double fSample,
         //const double fSignal,
         //const double voltage
         //):
   PulseGenerator< cic::OUTPUT_TYPE, cic::RESET_TYPE> 
   signal_generator( "pulse_gen", pulse_width, ipp_width, sample_rate, fsignal , 0.95*4095);
   signal_generator.output( dut_input_signal );
   signal_generator.reset( stimulus.reset );
   signal_generator.clock( stimulus.clock );

   //SinusoidGenerator< cic::OUTPUT_TYPE, cic::RESET_TYPE> 
   //signal_generator( "sig_gen", normalized_frequency , 12, 0.95);
   //signal_generator.output( dut_input_signal );
   //signal_generator.reset( stimulus.reset );
   //signal_generator.clock( stimulus.clock );

   // DUT
   Cic<cic::INPUT_TYPE,cic::OUTPUT_TYPE> cic( "cic" );
   cic.clock( stimulus.clock );
   cic.reset( stimulus.reset );
   cic.input( dut_input_signal );
   cic.output( dut_output_signal );
   cic.decimation( decimate_signal );

   // record output to file
   FileRecorder< cic::OUTPUT_TYPE, cic::RESET_TYPE> record( "record", RECORDER_FILE_NAME );
   record.input( dut_output_signal );
   record.reset( stimulus.reset );
   record.clock( cic.div_clock);

   // record output to file
   FileRecorder< cic::OUTPUT_TYPE, cic::RESET_TYPE> inputRecord( "input", "input.dat");
   inputRecord.input( dut_input_signal );
   inputRecord.reset( stimulus.reset );
   inputRecord.clock( stimulus.clock );

   // begin simulation
   sc_start( simulation_time );
   
   return 0;
}
