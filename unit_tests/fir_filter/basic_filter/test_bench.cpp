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
#include <list>

#include<yaml-cpp/yaml.h>

#include<sdr_simulator/filter/fir/FirFilter.hpp>
#include<sdr_simulator/output/FileRecorder.hpp>
#include<sdr_simulator/input/GaussianNoiseGenerator.hpp>
#include<sdr_simulator/input/Stimulus.hpp>

#include "test_bench.hpp"

using namespace std;

int sc_main(int argc, char* argv[])
{
   const string STIM_FILE_NAME   = "stimulus.yml";

   YAML::Node stim_node   = YAML::LoadFile( STIM_FILE_NAME )["stimulus"];

   // define constants
   const double TIME_RESOLUTION       = stim_node["time_resolution"].as<double>();
   const double TOTAL_SIMULATION_TIME = stim_node["simulation_time"].as<double>();
   const double SAMPLE_RATE           = stim_node["sample_rate"].as<double>();
   const double CLOCK_PERIOD          = 1.0e9 / SAMPLE_RATE;
   const unsigned int RESET_TIME      = stim_node["reset_time"].as<unsigned int>();
   const string OUTPUT_FILE_NAME      = stim_node["output_file"].as<string>();
   const double MEAN                  = stim_node["mean"].as<double>();
   const double VARIANCE              = stim_node["variance"].as<double>();
   const double AMPLITUDE             = stim_node["amplitude"].as<double>();

   // define signals
   sc_signal<data_input_type> input_signal;
   sc_signal<data_output_type> output_signal;

   // set time parameters
   sc_set_time_resolution( TIME_RESOLUTION , SC_NS );
   sc_time simulation_time(TOTAL_SIMULATION_TIME,SC_NS);
   sc_time clock_time(CLOCK_PERIOD,SC_NS);

   // Create a stimulus to generate clock and reset signals
   Stimulus<reset_type> stimulus("stimulus", clock_time, RESET_TIME );

   // Create a gaussian noise generator
   GaussianNoiseGenerator< data_input_type, reset_type>
      noiseGenerator_( "noise_generator", MEAN, VARIANCE, AMPLITUDE );
   noiseGenerator_.clock( stimulus.clock );
   noiseGenerator_.reset( stimulus.reset );
   noiseGenerator_.output( input_signal );

   // record output test data to file
   FileRecorder< data_output_type, reset_type > 
      record( "record", OUTPUT_FILE_NAME );
   record.input( output_signal );
   record.clock( stimulus.clock );
   record.reset( stimulus.reset );

   // Create a FirFilter object and generate coefficients
   typedef FirFilter< data_input_type, data_output_type, COEFF_WIDTH, ACCUMULATOR_WIDTH> FilterType;
   FilterType filter( "fir" );
   FilterType::CoeffVector coeff;

   // inefficient half-band filter implementation
   coeff.push_back(-49);
   coeff.push_back(0);
   coeff.push_back(165);
   coeff.push_back(0);
   coeff.push_back(-412);
   coeff.push_back(0);
   coeff.push_back(873);
   coeff.push_back(0);
   coeff.push_back(-1681);
   coeff.push_back(0);
   coeff.push_back(3135);
   coeff.push_back(0);
   coeff.push_back(-6282);
   coeff.push_back(0);
   coeff.push_back(20628);
   coeff.push_back(32767);
   coeff.push_back(20628);
   coeff.push_back(0);
   coeff.push_back(-6282);
   coeff.push_back(0);
   coeff.push_back(3135);
   coeff.push_back(0);
   coeff.push_back(-1681);
   coeff.push_back(0);
   coeff.push_back(873);
   coeff.push_back(0);
   coeff.push_back(-412);
   coeff.push_back(0);
   coeff.push_back(165);
   coeff.push_back(0);
   coeff.push_back(-49);
   
   // DUT
   filter.clock( stimulus.clock );
   filter.reset( stimulus.reset );
   filter.input( input_signal );
   filter.output( output_signal );
   filter.LoadCoefficients( coeff );

   // begin simulation
   sc_start( simulation_time );

   return EXIT_SUCCESS;
}
