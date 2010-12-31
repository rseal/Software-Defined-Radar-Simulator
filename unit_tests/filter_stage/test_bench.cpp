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
#include <fstream>

#include<systemc.h>

#include<sdr_simulator/receiver/usrp/FilterStage.hpp>
#include<sdr_simulator/util/FileRecorder.hpp>
#include<sdr_simulator/util/Stimulus.hpp>
#include<sdr_simulator/util/GaussianNoiseGenerator.hpp>
#include<sdr_simulator/util/SinusoidGenerator.hpp>

#include "configuration.hpp"

using namespace std;
using boost::lexical_cast;

int main()
{
   // define constants
   const double TIME_RESOLUTION = 1.0;
   const double TOTAL_SIMULATION_TIME = 1e5;
   const double CLOCK_PERIOD = 15.625;
   const unsigned int RESET_TIME = 10;
   const string INPHASE_FILE = "inphase.dat";
   const string QUADRATURE_FILE = "quadrature.dat";

   const double MEAN = 0.0;
   const double VARIANCE = 5.0;
   const double AMPLITUDE = 0.90;
   const int DECIMATION = 8.0;

   typedef GaussianNoiseGenerator< filter::INPUT_TYPE, filter::RESET_TYPE > NoiseGenerator;

   typedef usrp::FilterStage Filter;

   // define signals
   sc_signal< filter::INPUT_TYPE > inphase_signal_in;
   sc_signal< filter::OUTPUT_TYPE > inphase_signal_out;
   sc_signal< filter::INPUT_TYPE > quadrature_signal_in;
   sc_signal< filter::OUTPUT_TYPE > quadrature_signal_out;
   sc_signal< filter::DECIMATION_TYPE > decimation_signal;

   // set decimation
   decimation_signal.write( DECIMATION );

   // set time parameters
   sc_set_time_resolution( TIME_RESOLUTION , SC_NS );
   sc_time simulation_time(TOTAL_SIMULATION_TIME,SC_NS);
   sc_time clock_time(CLOCK_PERIOD,SC_NS);

   // create clock and reset stimuli
   Stimulus< filter::RESET_TYPE > stimulus("stimulus", clock_time, RESET_TIME );

   double sample_rate = 64e6;
   double frequency = 1e3;
   double normalized_frequency = frequency / ( sample_rate / DECIMATION );

   SinusoidGenerator< filter::INPUT_TYPE, filter::RESET_TYPE> 
   signal_generator( "sig_gen", normalized_frequency , 14, 0.95);
   signal_generator.output( inphase_signal_in );
   signal_generator.reset( stimulus.reset );
   signal_generator.clock( stimulus.clock );

   // Inphase noise signal
   //NoiseGenerator noisestimulus1( "noise_gen_1", MEAN, VARIANCE, AMPLITUDE );
   //noisestimulus1.clock( stimulus.clock );
   //noisestimulus1.reset( stimulus.reset );
   //noisestimulus1.output( inphase_signal_in );
   //noisestimulus1.Seed( 0 );

   // Quadrature noise signal
   NoiseGenerator noiseStimulus2( "noise_gen_2", MEAN, VARIANCE, AMPLITUDE );
   noiseStimulus2.clock( stimulus.clock );
   noiseStimulus2.reset( stimulus.reset );
   noiseStimulus2.output( quadrature_signal_in );
   noiseStimulus2.Seed( 1 );

   // USRP inphase filter
   Filter inphaseFilter("inphase");
   inphaseFilter.clock( stimulus.clock );
   inphaseFilter.reset( stimulus.reset );
   inphaseFilter.input( inphase_signal_in );
   inphaseFilter.output( inphase_signal_out ); 
   inphaseFilter.decimation( decimation_signal );

   // USRP quadrature filter
   Filter quadratureFilter("quadrature");
   quadratureFilter.clock( stimulus.clock );
   quadratureFilter.reset( stimulus.reset );
   quadratureFilter.input( quadrature_signal_in );
   quadratureFilter.output( quadrature_signal_out );
   quadratureFilter.decimation( decimation_signal );

   // record inphase data
   FileRecorder< filter::INPUT_TYPE, filter::RESET_TYPE > 
      inphaseRecord( "inphaseRecord", INPHASE_FILE);
   inphaseRecord.input( inphase_signal_out );
   inphaseRecord.clock( inphaseFilter.div_clock );
   inphaseRecord.reset( stimulus.reset );

   // record quadrature data
   FileRecorder< filter::INPUT_TYPE, filter::RESET_TYPE > 
      quadratureRecord( "quadratureRecord", QUADRATURE_FILE);
   quadratureRecord.input( quadrature_signal_out );
   quadratureRecord.clock( quadratureFilter.div_clock );
   quadratureRecord.reset( stimulus.reset );

   // record quadrature data
   FileRecorder< filter::INPUT_TYPE, filter::RESET_TYPE > 
      qInRecord( "qInput", "q_in.dat");
   qInRecord.input( quadrature_signal_in );
   qInRecord.clock( stimulus.clock );
   qInRecord.reset( stimulus.reset );

   // begin simulation
   sc_start( simulation_time );
}
