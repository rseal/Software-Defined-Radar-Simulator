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

#include <yaml-cpp/yaml.h>

#include <sdr_simulator/input/Stimulus.hpp>
#include <sdr_simulator/output/FileRecorder.hpp>
#include <sdr_simulator/input/GaussianNoiseGenerator.hpp>
#include <sdr_simulator/input/SinusoidGenerator.hpp>
#include <sdr_simulator/input/PulseGenerator.hpp>
#include <sdr_simulator/input/NoisySinusoidGenerator.hpp>

#include "Cic.hpp"
#include "configuration.hpp"

using namespace std;

int sc_main(int argc, char* argv[])
{
   const string STIMULUS_FILE = "stimulus.yml";

   try{
      YAML::Node root = YAML::LoadFile(STIMULUS_FILE)["stimulus"];

      const double TIME_RESOLUTION       = root["time_resolution"].as<double>();
      const double SAMPLE_RATE           = root["sample_rate"].as<double>();
      const double CLOCK_PERIOD          = 1.0e9/SAMPLE_RATE;
      const double TOTAL_SIMULATION_TIME = root["simulation_time"].as<double>();
      const unsigned int RESET_TIME      = root["reset_time"].as<unsigned int>();
      const int DECIMATION               = root["decimation"].as<int>();

      cout 
         << "TIME_RESOLUTION       : " << TIME_RESOLUTION << "\n"
         << "SAMPLE_RATE           : " << SAMPLE_RATE << "\n"
         << "CLOCK_PERIOD          : " << CLOCK_PERIOD << "\n"
         << "TOTAL_SIMULATION_TIME : " << TOTAL_SIMULATION_TIME << "\n"
         << "RESET_TIME            : " << RESET_TIME << "\n"
         << endl;

      const double PULSE_WIDTH      = root["pulse_width"].as<double>();
      const double IPP_WIDTH        = root["ipp_width"].as<double>();
      const double SIGNAL_FREQ      = root["signal_frequency"].as<double>();
      const string OUTPUT_DATA_FILE = root["output_file_name"].as<string>();
      const string INPUT_DATA_FILE  = root["input_file_name"].as<string>();

      sc_signal< cic::INPUT_TYPE > dut_input_signal;
      sc_signal< cic::OUTPUT_TYPE > dut_output_signal;
      sc_signal< cic::INPUT_TYPE > decimate_signal;

      // assign decimation value
      decimate_signal = cic::INPUT_TYPE(DECIMATION);

      // set time parameters
      sc_set_time_resolution( TIME_RESOLUTION, SC_PS );
      sc_time simulation_time( TOTAL_SIMULATION_TIME,SC_NS );
      sc_time clock_time(CLOCK_PERIOD,SC_NS);

      Stimulus< sdr::RESET_TYPE > stimulus("stimulus", clock_time, RESET_TIME );

      // gaussian noise source
      //GaussianNoiseGenerator< cic::OUTPUT_TYPE, sdr::RESET_TYPE > 
      // gaussianNoiseGen( "noise_gen", 0.0, 1.0, 0.25);
      //gaussianNoiseGen.output( dut_input_signal );
      //gaussianNoiseGen.reset( stimulus.reset );
      //gaussianNoiseGen.clock( stimulus.clock );

      //NoisySinusoidGenerator< cic::OUTPUT_TYPE, sdr::RESET_TYPE> 
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
      PulseGenerator< cic::OUTPUT_TYPE, sdr::RESET_TYPE> 
         signal_generator( "pulse_gen", PULSE_WIDTH, IPP_WIDTH, SAMPLE_RATE, SIGNAL_FREQ , 0.95*4095);
      signal_generator.output( dut_input_signal );
      signal_generator.reset( stimulus.reset );
      signal_generator.clock( stimulus.clock );

      //SinusoidGenerator< cic::OUTPUT_TYPE, sdr::RESET_TYPE> 
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
      FileRecorder< cic::OUTPUT_TYPE, sdr::RESET_TYPE> record( "record", OUTPUT_DATA_FILE );
      record.input( dut_output_signal );
      record.reset( stimulus.reset );
      record.clock( cic.div_clock);

      // record output to file
      FileRecorder< cic::OUTPUT_TYPE, sdr::RESET_TYPE> inputRecord( "input", INPUT_DATA_FILE);
      inputRecord.input( dut_input_signal );
      inputRecord.reset( stimulus.reset );
      inputRecord.clock( stimulus.clock );

      // begin simulation
      sc_start( simulation_time );
   }
   catch( YAML::ParserException& e)
   {
      cerr << e.what() << endl;
   }

   return 0;
}
