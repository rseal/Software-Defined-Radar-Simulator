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
#include <boost/lexical_cast.hpp>
#include <vector>

#include <sdr_simulator/filter/cic/CicIntegrator.hpp>
#include <sdr_simulator/filter/cic/CicDifferentiator.hpp>
#include <sdr_simulator/util/Stimulus.hpp>
#include <sdr_simulator/util/FileRecorder.hpp>

#include "test_bench.hpp"

using namespace std;
using boost::lexical_cast;

int sc_main(int argc, char* argv[] )
{
   const double TIME_RESOLUTION       = 1.0;
   const double TOTAL_SIMULATION_TIME = 5e5;
   const int RESET_TIME               = 10;
   const double CLOCK_PERIOD          = 2.0;
   const std::string OUTPUT_FILE_NAME = "data.dat";

   // set time parameters
   sc_set_time_resolution( TIME_RESOLUTION , SC_NS );
   sc_time simulation_time(TOTAL_SIMULATION_TIME,SC_NS);

   sc_time clock_time(CLOCK_PERIOD,SC_NS);
   sc_time clock_time2(CLOCK_PERIOD*2,SC_NS);

   sc_signal<INPUT_TYPE> input_signal;
   sc_signal<OUTPUT_TYPE> cic_output_signal;

   sc_signal<OUTPUT_TYPE> stage_01_signal;
   sc_signal<OUTPUT_TYPE> stage_12_signal;
   sc_signal<OUTPUT_TYPE> stage_23_signal;

   sc_signal<OUTPUT_TYPE> diff_01_signal;
   sc_signal<OUTPUT_TYPE> diff_12_signal;
   sc_signal<OUTPUT_TYPE> diff_23_signal;

   // input DC signal into integrator
   input_signal.write( 1 );

   // create clock and reset stimulus
   Stimulus<RESET_TYPE> stimulus( "stimulus", clock_time, RESET_TIME );
   Stimulus<RESET_TYPE> stimulus2( "stimulus", clock_time2, RESET_TIME );


   // DUT instantiation
   CicIntegrator< INPUT_WIDTH, OUTPUT_WIDTH > cic_integrator_0( "cic_integrator_0" );
   cic_integrator_0.clock( stimulus.clock );
   cic_integrator_0.reset( stimulus.reset );
   cic_integrator_0.input( input_signal );
   cic_integrator_0.output( stage_01_signal );

   // Record output signal to file.
   FileRecorder< OUTPUT_TYPE, RESET_TYPE> record_0( "record_0", "out_0.dat" );
   record_0.clock( stimulus.clock );
   record_0.reset( stimulus.reset );
   record_0.input( stage_01_signal );

   // DUT instantiation
   CicIntegrator< INPUT_WIDTH, OUTPUT_WIDTH > cic_integrator_1( "cic_integrator_1" );
   cic_integrator_1.clock( stimulus.clock );
   cic_integrator_1.reset( stimulus.reset );
   cic_integrator_1.input( stage_01_signal );
   cic_integrator_1.output( stage_12_signal );

   // Record output signal to file.
   FileRecorder< OUTPUT_TYPE, RESET_TYPE> record_1( "record_1", "out_1.dat" );
   record_1.clock( stimulus.clock );
   record_1.reset( stimulus.reset );
   record_1.input( stage_12_signal );

   // DUT instantiation
   CicIntegrator< INPUT_WIDTH, OUTPUT_WIDTH > cic_integrator_2( "cic_integrator_2" );
   cic_integrator_2.clock( stimulus.clock );
   cic_integrator_2.reset( stimulus.reset );
   cic_integrator_2.input( stage_12_signal );
   cic_integrator_2.output( stage_23_signal );

   // Record output signal to file.
   FileRecorder< OUTPUT_TYPE, RESET_TYPE> record_2( "record_2", "out_2.dat" );
   record_2.clock( stimulus.clock );
   record_2.reset( stimulus.reset );
   record_2.input( stage_23_signal );

   // DUT instantiation
   CicIntegrator< INPUT_WIDTH, OUTPUT_WIDTH > cic_integrator_3( "cic_integrator_3" );
   cic_integrator_3.clock( stimulus.clock );
   cic_integrator_3.reset( stimulus.reset );
   cic_integrator_3.input( stage_23_signal );
   cic_integrator_3.output( cic_output_signal );

   // Record output signal to file.
   FileRecorder< OUTPUT_TYPE, RESET_TYPE> record_3( "record_3", "out_3.dat" );
   record_3.clock( stimulus.clock );
   record_3.reset( stimulus.reset );
   record_3.input( cic_output_signal );

   CicDifferentiator< INPUT_WIDTH, OUTPUT_WIDTH > cic_differentiator_0( "cic_diff_0" );
   cic_differentiator_0.clock( stimulus2.clock );
   cic_differentiator_0.reset( stimulus2.reset );
   cic_differentiator_0.input( cic_output_signal );
   cic_differentiator_0.output( diff_01_signal );

   // Record output signal to file.
   FileRecorder< OUTPUT_TYPE, RESET_TYPE> record_4( "record_4", "out_4.dat" );
   record_4.clock( stimulus2.clock );
   record_4.reset( stimulus2.reset );
   record_4.input( diff_01_signal );

   CicDifferentiator< INPUT_WIDTH, OUTPUT_WIDTH > cic_differentiator_1( "cic_diff_1" );
   cic_differentiator_1.clock( stimulus2.clock );
   cic_differentiator_1.reset( stimulus2.reset );
   cic_differentiator_1.input( diff_01_signal );
   cic_differentiator_1.output( diff_12_signal );

   // Record output signal to file.
   FileRecorder< OUTPUT_TYPE, RESET_TYPE> record_5( "record_5", "out_5.dat" );
   record_5.clock( stimulus2.clock );
   record_5.reset( stimulus2.reset );
   record_5.input( diff_12_signal );

   CicDifferentiator< INPUT_WIDTH, OUTPUT_WIDTH > cic_differentiator_2( "cic_diff_2" );
   cic_differentiator_2.clock( stimulus2.clock );
   cic_differentiator_2.reset( stimulus2.reset );
   cic_differentiator_2.input( diff_12_signal );
   cic_differentiator_2.output( diff_23_signal );

   // Record output signal to file.
   FileRecorder< OUTPUT_TYPE, RESET_TYPE> record_6( "record_6", "out_6.dat" );
   record_6.clock( stimulus2.clock );
   record_6.reset( stimulus2.reset );
   record_6.input( diff_23_signal );

   // begin simulation
   sc_start( simulation_time );
}
