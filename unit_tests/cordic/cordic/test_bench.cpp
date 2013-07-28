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

#include <boost/shared_ptr.hpp>

#include <systemc.h>
#include <yaml-cpp/yaml.h>

#include <sdr_simulator/down_converter/cordic/Cordic.hpp>
#include <sdr_simulator/input/Stimulus.hpp>
#include <sdr_simulator/input/SinusoidGenerator.hpp>
#include <sdr_simulator/input/NoisySinusoidGenerator.hpp>
#include <sdr_simulator/misc/PhaseAccumulator.hpp>
#include <sdr_simulator/output/FileRecorder.hpp>

#include "configuration.hpp"

using namespace std;

int sc_main ( int argc, char* argv[] )
{
   YAML::Node stim_node = YAML::LoadFile("stimulus.yml")["stimulus"];

   const double TIME_RESOLUTION       = stim_node["time_resolution"].as<double>();
   const int RESET_TIME               = stim_node["reset_time"].as<int>();
   const double TOTAL_SIMULATION_TIME = stim_node["simulation_time"].as<double>();
   const double SIGNAL_FREQUENCY      = stim_node["signal_frequency"].as<double>();
   const double DDC_FREQUENCY         = stim_node["ddc_frequency"].as<double>();
   const double SAMPLE_RATE           = stim_node["sample_rate"].as<double>();
   const double NORMALIZED_FREQUENCY  = SIGNAL_FREQUENCY/SAMPLE_RATE;
   const double SCALE                 = tr1::pow(2.0, accumulator::INPUT_WIDTH);
   const int STEP_SIZE                = rint( SCALE*DDC_FREQUENCY/SAMPLE_RATE );
   const string INPUT_DATA_FILE       = stim_node["input_file_name"].as<string>();
   const double CLOCK_PERIOD          = 1.0e9 / SAMPLE_RATE;
   const unsigned int  ADC_WIDTH      = stim_node["adc_width"].as<unsigned int>();
   const double  ADC_MEAN             = stim_node["mean"].as<double>();
   const double  ADC_VAR              = stim_node["variance"].as<double>();
   const double  ADC_AMP              = stim_node["amplitude"].as<double>();

   const string X_DATA_FILE_NAME = "xout.dat";
   const string Y_DATA_FILE_NAME = "yout.dat";
   const string Z_DATA_FILE_NAME = "zout.dat";

   cout << " PARAMETERS \n"
      << "SIGNAL_FREQUENCY     : " << SIGNAL_FREQUENCY     << "\n"
      << "DDC_FREQUENCY        : " << DDC_FREQUENCY        << "\n"
      << "SAMPLE_RATE          : " << SAMPLE_RATE          << "\n"
      << "NORMALIZED_FREQUENCY : " << NORMALIZED_FREQUENCY << "\n";

   // set time parameters
   sc_set_time_resolution  ( TIME_RESOLUTION       , SC_PS )  ; 
   sc_time clock_time      ( CLOCK_PERIOD          , SC_NS )  ; 
   sc_time simulation_time ( TOTAL_SIMULATION_TIME , SC_NS )  ; 

   // interface signals
   sc_signal<sdr::RESET_TYPE> reset_signal;
   sc_signal<ddc::OUTPUT_TYPE> x_data_signal;
   sc_signal<ddc::OUTPUT_TYPE> y_data_signal;
   sc_signal<ddc::PHASE_TYPE> phase_data_signal;
   sc_signal<ddc::OUTPUT_TYPE> x_data_output_signal;
   sc_signal<ddc::OUTPUT_TYPE> y_data_output_signal;
   sc_signal<accumulator::OUTPUT_TYPE> z_data_output_signal;

   Stimulus< sdr::RESET_TYPE > stimulus ( "stimulus", clock_time, RESET_TIME );

   // the y-input to the CORDIC is constant. This will
   // peform down-conversion and provide a complex output.
   y_data_signal.write ( 0 );

   NoisySinusoidGenerator<ddc::OUTPUT_TYPE, sdr::RESET_TYPE>
   sig_gen("sig_gen", NORMALIZED_FREQUENCY, ADC_WIDTH, ADC_MEAN, ADC_VAR, ADC_AMP);
   sig_gen.reset ( stimulus.reset );
   sig_gen.clock ( stimulus.clock );
   sig_gen.output ( x_data_signal );

   // Store the input signal into a file.
   FileRecorder< ddc::OUTPUT_TYPE, sdr::RESET_TYPE > 
      inputSignalRecorder ("input_signal_data_recorder", INPUT_DATA_FILE );
   inputSignalRecorder.clock ( stimulus.clock );
   inputSignalRecorder.reset ( stimulus.reset );
   inputSignalRecorder.input ( x_data_signal );

   // Create an accumulator object to feed into the theta ( phase ) input.
   PhaseAccumulator<accumulator::INPUT_TYPE, accumulator::OUTPUT_TYPE> 
      phaseAccumulator ("phase_accumulator" );
   phaseAccumulator.StepSize(STEP_SIZE);
   phaseAccumulator.clock ( stimulus.clock );
   phaseAccumulator.reset ( stimulus.reset );
   phaseAccumulator.output ( phase_data_signal );

   // create cordic_stage object ( DUT )
   typedef Cordic<
      ddc::INPUT_TYPE, 
      ddc::OUTPUT_TYPE, 
      accumulator::OUTPUT_TYPE, 
		sdr::RESET_TYPE, 
      ddc::DATA_WIDTH, 
      ddc::PHASE_WIDTH, 
      ddc::NUM_STAGES> CordicObj;
   CordicObj dut( "cordic" );
   dut.clock ( stimulus.clock );
   dut.reset ( stimulus.reset );
   dut.real_input ( x_data_signal );
   dut.imag_input ( y_data_signal );
   dut.phase_input ( phase_data_signal );
   dut.real_output ( x_data_output_signal );
   dut.imag_output ( y_data_output_signal );
   dut.phase_output ( z_data_output_signal );

   FileRecorder< ddc::INPUT_TYPE, sdr::RESET_TYPE > inputRecorder (
         "input_recorder", "input_signal.dat");
   inputRecorder.clock ( stimulus.clock );
   inputRecorder.reset ( stimulus.reset );
   inputRecorder.input ( x_data_signal );

   // Store the x data output into a file.
   FileRecorder< ddc::OUTPUT_TYPE, sdr::RESET_TYPE > xDataRecorder (
         "x_data_recorder", X_DATA_FILE_NAME );
   xDataRecorder.clock ( stimulus.clock );
   xDataRecorder.reset ( stimulus.reset );
   xDataRecorder.input ( x_data_output_signal );

   // Store the y data output into a file.
   FileRecorder< ddc::OUTPUT_TYPE, sdr::RESET_TYPE > yDataRecorder (
         "y_data_recorder", Y_DATA_FILE_NAME );
   yDataRecorder.clock ( stimulus.clock );
   yDataRecorder.reset ( stimulus.reset );
   yDataRecorder.input ( y_data_output_signal );

   // Store the z data output into a file.
   FileRecorder< accumulator::OUTPUT_TYPE, sdr::RESET_TYPE > zDataRecorder (
         "z_data_recorder", Z_DATA_FILE_NAME );
   zDataRecorder.clock ( stimulus.clock );
   zDataRecorder.reset ( stimulus.reset );
   zDataRecorder.input ( z_data_output_signal );

   // begin simulation
   sc_start ( simulation_time );

   return 0;
}
