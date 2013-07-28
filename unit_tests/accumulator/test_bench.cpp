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

#include <cmath>
#include <sdr_simulator/misc/PhaseAccumulator.hpp>
#include <sdr_simulator/output/FileRecorder.hpp>
#include <sdr_simulator/input/Stimulus.hpp>

#include <iostream>
#include <boost/math/constants/constants.hpp>
#include <boost/lexical_cast.hpp>

#include<yaml-cpp/yaml.h>

// this file is auto-generated
#include "configuration.hpp"

using namespace std;
using namespace boost::math;
using namespace boost;

int sc_main(int argc, char* argv[]){

   YAML::Node stim_node = YAML::LoadFile("stimulus.yml")["stimulus"];

   // assign dynamic parameters
   const int RESET_TIME            = stim_node["reset_time"].as<double>();
   const double SIMULATION_TIME    = stim_node["simulation_time"].as<double>();
   const double TIME_RESOLUTION    = stim_node["time_resolution"].as<double>();
   const string RECORDER_FILE_NAME = stim_node["output_file_name"].as<string>();
   const double SAMPLE_RATE        = stim_node["sample_rate"].as<double>();
   const double DDC_RATE           = stim_node["ddc_rate"].as<double>();
   const double STEP_SIZE          = pow(2.0,accumulator::INPUT_WIDTH) * DDC_RATE/ SAMPLE_RATE;
   const double CLOCK_PERIOD       = 1/SAMPLE_RATE*1e9;

   // generate signals
   sc_signal< accumulator::OUTPUT_TYPE > output_signal;

   // set time parameters
   sc_set_time_resolution ( TIME_RESOLUTION , SC_PS );
   sc_time time( CLOCK_PERIOD, SC_NS );
   sc_time simulation_time ( SIMULATION_TIME,SC_NS );

   // display settings
   cout 
      << "\nsample rate      = " << SAMPLE_RATE
      << "\nclock period     = " << CLOCK_PERIOD
      << "\ninput width      = " << accumulator::INPUT_WIDTH
      << "\noutput width     = " << accumulator::OUTPUT_WIDTH
      << "\nreset time       = " << RESET_TIME
      << "\nsimulation time  = " << SIMULATION_TIME
      << "\ntime resolution  = " << TIME_RESOLUTION
      << "\noutput file name = " << RECORDER_FILE_NAME
      << "\nddc rate         = " << DDC_RATE
      << "\nstep size        = " << STEP_SIZE
      << "\n\n";

   // define testbench stimulus
   Stimulus< sdr::reset_type > stimulus( "stimulus", time, RESET_TIME );

   // DUT
   PhaseAccumulator<accumulator::INPUT_TYPE,accumulator::OUTPUT_TYPE> accumulator("accumulator");
   accumulator.StepSize(STEP_SIZE);
   accumulator.clock( stimulus.clock );
   accumulator.output(output_signal);
   accumulator.reset( stimulus.reset );

   // record output
   FileRecorder< accumulator::OUTPUT_TYPE, sdr::reset_type > record( "recorder", RECORDER_FILE_NAME );
   record.clock( stimulus.clock );
   record.reset( stimulus.reset );
   record.input( output_signal );

   //run simulations for 22 nsec
   sc_start ( simulation_time );

   return EXIT_SUCCESS;
}
