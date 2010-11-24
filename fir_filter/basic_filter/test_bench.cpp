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

#include<sdr_simulator/FirFilter.hpp>
#include<sdr_simulator/util/FileRecorder.hpp>
#include<sdr_simulator/util/GaussianNoiseGenerator.hpp>
#include<sdr_simulator/util/Stimulus.hpp>


#include "test_bench.hpp"

using namespace std;

int main()
{
   // define constants
   const double TIME_RESOLUTION = 1.0;
   const double TOTAL_SIMULATION_TIME = 500000.0;
   const double CLOCK_PERIOD = 2.0;
   const unsigned int RESET_TIME = 10;
   const string OUTPUT_FILE_NAME = "output.dat";

   // Create a set of filter coefficients for testing DUT
   // NOTE: this test is far from optimal, but only used 
   // to for initial development at this point.
   typedef sc_int< COEFF_WIDTH > coeff_type;
   typedef std::list< coeff_type > CoefficientList;
   CoefficientList coeff;

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
   

   // define signals
   sc_signal< data_input_type > input_signal;
   sc_signal< data_output_type > output_signal;

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

   // DUT
   FirFilter< INPUT_WIDTH, OUTPUT_WIDTH, COEFF_WIDTH, ACCUMULATOR_WIDTH > filter( "fir");
   filter.Initialize( coeff );
   filter.clock( stimulus.clock );
   filter.reset( stimulus.reset );
   filter.input( input_signal );
   filter.output( output_signal );

   // record output test data to file
   FileRecorder< data_output_type, reset_type > 
      record( "record", OUTPUT_FILE_NAME );
   record.input( output_signal );
   record.clock( stimulus.clock );
   record.reset( stimulus.reset );

   // begin simulation
   sc_start( simulation_time );
}
