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

#include <yaml-cpp/yaml.h>

#include <sdr_simulator/filter/cic/CicIntegrator.hpp>
#include <sdr_simulator/filter/cic/CicDifferentiator.hpp>
#include <sdr_simulator/input/Stimulus.hpp>
#include <sdr_simulator/input/ClockDivider.hpp>
#include <sdr_simulator/input/ImpulseGenerator.hpp>
#include <sdr_simulator/output/FileRecorder.hpp>

using namespace std;
using boost::lexical_cast;

int sc_main(int argc, char* argv[] )
{
   const string STIMULUS_FILE = "stimulus.yml";
   sc_trace_file* fout = sc_create_vcd_trace_file("test");

   try
   {
      YAML::Node root = YAML::LoadFile(STIMULUS_FILE)["stimulus"];

      const double TIME_RESOLUTION       = root["time_resolution"].as<double>();
      const double SAMPLE_RATE           = root["sample_rate"].as<double>();
      const double CLOCK_PERIOD          = 1.0e9/SAMPLE_RATE;
      const double TOTAL_SIMULATION_TIME = root["simulation_time"].as<double>();
      const double RESET_TIME            = root["reset_time"].as<double>() * CLOCK_PERIOD;
      const string OUTPUT_FILE_BASE      = root["output_base_name"].as<string>();
      const double AMPLITUDE             = root["amplitude"].as<double>();
      const int DECIMATION               = root["decimation"].as<int>();
      const int NUM_SAMPLES              = 1.0e3*TOTAL_SIMULATION_TIME/TIME_RESOLUTION;

      cout 
          << "TIME_RESOLUTION       : " << TIME_RESOLUTION       << "\n"
          << "SAMPLE_RATE           : " << SAMPLE_RATE           << "\n"
          << "CLOCK_PERIOD          : " << CLOCK_PERIOD          << "\n"
          << "TOTAL_SIMULATION_TIME : " << TOTAL_SIMULATION_TIME << "\n"
          << "RESET_TIME            : " << RESET_TIME            << "\n"
          << "NUM_SAMPLES           : " << NUM_SAMPLES           << "\n"
          << "DECIMATION            : " << DECIMATION            << "\n"
          << endl;


      const int S0 = 28;
      const int S1 = 28;
      const int S2 = 28;
      const int S3 = 28;
      const int S4 = 28;

      typedef bool RESET_TYPE;

      typedef sc_int<S0> S0_TYPE;
      typedef sc_int<S1> S1_TYPE;
      typedef sc_int<S2> S2_TYPE;
      typedef sc_int<S3> S3_TYPE;
      typedef sc_int<S4> S4_TYPE;

      // set time parameters
      //sc_set_time_resolution( TIME_RESOLUTION , SC_NS );
      sc_time simulation_time(TOTAL_SIMULATION_TIME,SC_NS);

      sc_time clock_time(CLOCK_PERIOD,SC_NS);
      sc_time clock2_time(CLOCK_PERIOD*DECIMATION,SC_NS);

      sc_signal<S0_TYPE> s0_signal;
      sc_signal<S1_TYPE> s1_signal;
      sc_signal<S2_TYPE> s2_signal;
      sc_signal<S3_TYPE> s3_signal;
      sc_signal<S4_TYPE> s4_signal;

      // create clock and reset stimulus
      Stimulus<RESET_TYPE> stimulus( "stimulus0", clock_time, RESET_TIME );
      Stimulus<RESET_TYPE> stimulus1( "stimulus1", clock2_time, RESET_TIME*DECIMATION+2*RESET_TIME );

      ClockDivider div_clock("div_clk", DECIMATION);
      div_clock.clock( stimulus.clock );
      div_clock.reset( stimulus1.reset );

      ImpulseGenerator<S0_TYPE, RESET_TYPE> 
      sig_gen("sig_gen", NUM_SAMPLES, 1.0);
      sig_gen.output( s0_signal );
      sig_gen.reset( stimulus.reset );
      sig_gen.clock( stimulus.clock );

      // DUT instantiation
      CicIntegrator< S0, S1 > cic_integrator0( "cic_integrator0" );
      cic_integrator0.clock( stimulus.clock );
      cic_integrator0.reset( stimulus.reset );
      cic_integrator0.input( s0_signal );
      cic_integrator0.output( s1_signal );

      CicIntegrator< S1, S2 > cic_integrator1( "cic_integrator1" );
      cic_integrator1.clock( stimulus.clock );
      cic_integrator1.reset( stimulus.reset );
      cic_integrator1.input( s1_signal );
      cic_integrator1.output( s2_signal );

      CicDifferentiator<S2, S3> diff_0("diff_0");
      diff_0.clock( div_clock.div_clock);
      diff_0.reset( stimulus1.reset );
      diff_0.input( s2_signal );
      diff_0.output( s3_signal );

      CicDifferentiator<S3, S4> diff_1("diff_1");
      diff_1.clock( div_clock.div_clock);
      diff_1.reset( stimulus1.reset );
      diff_1.input( s3_signal );
      diff_1.output( s4_signal );

      // Record output signal to file.
      FileRecorder< S1_TYPE, RESET_TYPE> record_s0( "record0", "input.dat" );
      record_s0.clock( stimulus.clock );
      record_s0.reset( stimulus.reset );
      record_s0.input( s0_signal );

      // Record output signal to file.
      FileRecorder< S1_TYPE, RESET_TYPE> record_s1( "record1", "out0.dat" );
      record_s1.clock( stimulus.clock );
      record_s1.reset( stimulus.reset );
      record_s1.input( s1_signal );

      // Record output signal to file.
      FileRecorder< S2_TYPE, RESET_TYPE> record_s2( "record2", "out1.dat" );
      record_s2.clock( stimulus.clock );
      record_s2.reset( stimulus.reset );
      record_s2.input( s2_signal );

      // Record output signal to file.
      FileRecorder< S3_TYPE, RESET_TYPE> record_s3( "record3", "out2.dat" );
      record_s3.clock( stimulus.clock );
      record_s3.reset( stimulus.reset );
      record_s3.input( s3_signal );

      // Record output signal to file.
      FileRecorder< S4_TYPE, RESET_TYPE> record_s4( "record4", "out3.dat" );
      record_s4.clock( stimulus.clock );
      record_s4.reset( stimulus.reset );
      record_s4.input( s4_signal );

      //sc_trace(fout, stimulus.clock, "stim.clock");
      sc_trace(fout, stimulus, "stim");
      sc_trace(fout, stimulus1, "stim2");
      sc_trace(fout, s0_signal, "sig0");
      sc_trace(fout, s1_signal, "sig1");
      sc_trace(fout, s2_signal, "sig2");
      sc_trace(fout, s3_signal, "sig3");
      sc_trace(fout, s4_signal, "sig4");
      sc_trace(fout, div_clock, "clk_div");

      // begin simulation
      sc_start( simulation_time );

   }
   catch( YAML::ParserException& e)
   {
      cerr << e.what() << endl;
   }

   sc_close_vcd_trace_file(fout);

   return EXIT_SUCCESS;
}
