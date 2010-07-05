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
#include <boost/lexical_cast.hpp>
#include <list>

#include<sdr_simulator/FirFilter.hpp>
#include<sdr_simulator/Types.hpp>
#include<sdr_simulator/util/FileRecorder.hpp>
#include<sdr_simulator/util/GaussianNoiseStimulus.hpp>

#include "test_bench.hpp"

using namespace std;
using boost::lexical_cast;

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
   typedef sc_int< testbench::COEFF_WIDTH > coeff_type;
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
   sdr_types::reset_signal reset_signal;
   sc_signal< testbench::data_input_type > input_signal;
   sc_signal< testbench::data_output_type > output_signal;
   sc_signal< bool > clk_signal;

   // set time parameters
   sc_set_time_resolution( TIME_RESOLUTION , SC_NS );
   sc_time simulation_time(TOTAL_SIMULATION_TIME,SC_NS);
   sc_time clock_time(CLOCK_PERIOD,SC_NS);
   sc_clock stimulus_clock( "clock", clock_time );

   // create stimulus signals for DUT
   GaussianNoiseStimulus< testbench::data_input_type, testbench::INPUT_WIDTH >
      stimulus( "stimulus", RESET_TIME, stimulus_clock, 0.0, 1.0, 0.25 );
   stimulus.output( output_signal );

   // record output test data to file
   FileRecorder< testbench::data_input_type > 
      record( "record", OUTPUT_FILE_NAME );
   record.input( input_signal );
   record.clock( stimulus.clock );

   // DUT
   FirFilter< testbench::INPUT_WIDTH, testbench::OUTPUT_WIDTH, 
      testbench::COEFF_WIDTH, testbench::SUM_WIDTH > filter( "fir");
   filter.Initialize( coeff );
   filter.clock( stimulus.clock );
   filter.reset( stimulus.reset );
   filter.input( output_signal );
   filter.output( input_signal );

   // begin simulation
   sc_start( simulation_time );
}
