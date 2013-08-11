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
#include <vector>
#include <algorithm>

#include <boost/lexical_cast.hpp>
#include <tr1/math.h>

#include <yaml-cpp/yaml.h>

#include <sdr_simulator/filter/cic/CicIntegrator.hpp>
#include <sdr_simulator/filter/cic/CicDifferentiator.hpp>
#include <sdr_simulator/input/Stimulus.hpp>
#include <sdr_simulator/input/ClockDivider.hpp>
#include <sdr_simulator/input/ImpulseGenerator.hpp>
#include <sdr_simulator/input/NoisySinusoidGenerator.hpp>
#include <sdr_simulator/output/FileRecorder.hpp>
#include <sdr_simulator/output/BitAdjust.hpp>

using namespace std;
using boost::lexical_cast;

int sc_main(int argc, char* argv[] )
{
   const string STIMULUS_FILE = "stimulus.yml";
   sc_trace_file* fout = sc_create_vcd_trace_file("test");

   try
   {
      YAML::Node root = YAML::LoadFile(STIMULUS_FILE)["stimulus"];

      const int NUM_STAGES = 5;

      const double TIME_RESOLUTION       = root["time_resolution"].as<double>();
      const double SAMPLE_RATE           = root["sample_rate"].as<double>();
      const double CLOCK_PERIOD          = 1.0e9/SAMPLE_RATE;
      const double TOTAL_SIMULATION_TIME = root["simulation_time"].as<double>();
      const double RESET_TIME            = root["reset_time"].as<double>() * CLOCK_PERIOD;
      const string OUTPUT_FILE_BASE      = root["output_base_name"].as<string>();
      const double AMPLITUDE             = root["amplitude"].as<double>();
      const int DECIMATION               = root["decimation"].as<int>();
      const int MIN_DECIMATION           = root["min_decimation"].as<int>();
      const int MAX_DECIMATION           = root["max_decimation"].as<int>();
      const int NUM_SAMPLES              = 1.0e3*TOTAL_SIMULATION_TIME/TIME_RESOLUTION;
      const int START_IDX                = root["start_index"].as<int>();
      const double SIGNAL_FREQUENCY      = root["signal_frequency"].as<double>();
      const int ADC_WIDTH                = root["adc_width"].as<int>();
      const double MEAN                  = root["mean"].as<double>();
      const double VARIANCE              = root["variance"].as<double>();
      const int BIT_GAIN                 = std::tr1::ceil(NUM_STAGES*std::tr1::log2(DECIMATION));
      const int MAX_GAIN                 = std::tr1::ceil(NUM_STAGES*std::tr1::log2(MAX_DECIMATION));

      cout 
          << "TIME_RESOLUTION       : " << TIME_RESOLUTION       << "\n"
          << "SAMPLE_RATE           : " << SAMPLE_RATE           << "\n"
          << "CLOCK_PERIOD          : " << CLOCK_PERIOD          << "\n"
          << "TOTAL_SIMULATION_TIME : " << TOTAL_SIMULATION_TIME << "\n"
          << "RESET_TIME            : " << RESET_TIME            << "\n"
          << "NUM_SAMPLES           : " << NUM_SAMPLES           << "\n"
          << "DECIMATION            : " << DECIMATION            << "\n"
          << "BIT_GAIN              : " << BIT_GAIN              << "\n"
          << endl;


      // min_dec = 48, max_dec = 64
      //const int S0  = 44;
      //const int S1  = 38;
      //const int S2  = 32;
      //const int S3  = 28;
      //const int S4  = 23;
      //const int S5  = 21;
      //const int S6  = 20;
      //const int S7  = 19;
      //const int S8  = 18;
      //const int S9  = 18;
      //const int S10 = 16;
      //const int S11 = 12;

      //const int S0  = 45;
      //const int S1  = 42; 
      //const int S2  = 40; 
      //const int S3  = 38; 
      //const int S4  = 37;
      //const int S5  = 36;
      //const int S6  = 35;
      //const int S7  = 34;
      //const int S8  = 33;
      //const int S9  = 31;
      //const int S10 = 30;
      //const int S11 = 16;


      const int S0  = 46;
      const int S1  = 43;
      const int S2  = 41;
      const int S3  = 39;
      const int S4  = 38;
      const int S5  = 37;
      const int S6  = 36;
      const int S7  = 35;
      const int S8  = 34;
      const int S9  = 34;
      const int S10 = 31;
      const int S11 = 16;

      const int BIT_TRIM = S0-S10;

      typedef bool RESET_TYPE;

      typedef sc_int < S0  > S0_TYPE  ; 
      typedef sc_int < S1  > S1_TYPE  ; 
      typedef sc_int < S2  > S2_TYPE  ; 
      typedef sc_int < S3  > S3_TYPE  ; 
      typedef sc_int < S4  > S4_TYPE  ; 
      typedef sc_int < S5  > S5_TYPE  ; 
      typedef sc_int < S6  > S6_TYPE  ; 
      typedef sc_int < S7  > S7_TYPE  ; 
      typedef sc_int < S8  > S8_TYPE  ; 
      typedef sc_int < S9  > S9_TYPE  ; 
      typedef sc_int < S10  > S10_TYPE  ; 
      typedef sc_int < S11  > S11_TYPE  ; 

      sc_signal < S0_TYPE  > s0_signal  ; 
      sc_signal < S1_TYPE  > s1_signal  ; 
      sc_signal < S2_TYPE  > s2_signal  ; 
      sc_signal < S3_TYPE  > s3_signal  ; 
      sc_signal < S4_TYPE  > s4_signal  ; 
      sc_signal < S5_TYPE  > s5_signal  ; 
      sc_signal < S6_TYPE  > s6_signal  ; 
      sc_signal < S7_TYPE  > s7_signal  ; 
      sc_signal < S8_TYPE  > s8_signal  ; 
      sc_signal < S9_TYPE  > s9_signal  ; 
      sc_signal < S10_TYPE  > s10_signal  ; 
      sc_signal < S11_TYPE  > s11_signal  ; 

      // set time parameters
      //sc_set_time_resolution( TIME_RESOLUTION , SC_NS );
      sc_time simulation_time(TOTAL_SIMULATION_TIME,SC_NS);

      sc_time clock_time(CLOCK_PERIOD,SC_NS);
      sc_time clock2_time(CLOCK_PERIOD*DECIMATION,SC_NS);

      // create clock and reset stimulus
      Stimulus<RESET_TYPE> stimulus( "stimulus0", clock_time, RESET_TIME );
      Stimulus<RESET_TYPE> stimulus1( "stimulus1", clock2_time, RESET_TIME*DECIMATION+2*RESET_TIME );

      ClockDivider div_clock("div_clk", DECIMATION);
      div_clock.clock( stimulus.clock );
      div_clock.reset( stimulus1.reset );


      //s0_signal.write(1);

      
      NoisySinusoidGenerator< S0_TYPE, S0_TYPE>
      sig_gen( "sig_gen", SIGNAL_FREQUENCY/SAMPLE_RATE, ADC_WIDTH, MEAN, VARIANCE, AMPLITUDE);
      sig_gen.output( s0_signal );
      sig_gen.reset( stimulus.reset );
      sig_gen.clock( stimulus.clock );


      //ImpulseGenerator<S0_TYPE, RESET_TYPE> 
      //sig_gen("sig_gen", NUM_SAMPLES, START_IDX, AMPLITUDE);
      //sig_gen.output( s0_signal );
      //sig_gen.reset( stimulus.reset );
      //sig_gen.clock( stimulus.clock );

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

      CicIntegrator< S2, S3 > cic_integrator2( "cic_integrator2" );
      cic_integrator2.clock( stimulus.clock );
      cic_integrator2.reset( stimulus.reset );
      cic_integrator2.input( s2_signal );
      cic_integrator2.output( s3_signal );

      CicIntegrator< S3, S4 > cic_integrator3( "cic_integrator3" );
      cic_integrator3.clock( stimulus.clock );
      cic_integrator3.reset( stimulus.reset );
      cic_integrator3.input( s3_signal );
      cic_integrator3.output( s4_signal );

      CicIntegrator< S4, S5 > cic_integrator4( "cic_integrator4" );
      cic_integrator4.clock( stimulus.clock );
      cic_integrator4.reset( stimulus.reset );
      cic_integrator4.input( s4_signal );
      cic_integrator4.output( s5_signal );

      CicDifferentiator<S5, S6> diff_0("diff_0");
      diff_0.clock( div_clock.div_clock);
      diff_0.reset( stimulus1.reset );
      diff_0.input( s5_signal );
      diff_0.output( s6_signal );

      CicDifferentiator<S6, S7> diff_1("diff_1");
      diff_1.clock( div_clock.div_clock);
      diff_1.reset( stimulus1.reset );
      diff_1.input( s6_signal );
      diff_1.output( s7_signal );

      CicDifferentiator<S7, S8> diff_2("diff_2");
      diff_2.clock( div_clock.div_clock);
      diff_2.reset( stimulus1.reset );
      diff_2.input( s7_signal );
      diff_2.output( s8_signal );

      CicDifferentiator<S8, S9> diff_3("diff_3");
      diff_3.clock( div_clock.div_clock);
      diff_3.reset( stimulus1.reset );
      diff_3.input( s8_signal );
      diff_3.output( s9_signal );

      CicDifferentiator<S9, S10> diff_4("diff_4");
      diff_4.clock( div_clock.div_clock);
      diff_4.reset( stimulus1.reset );
      diff_4.input( s9_signal );
      diff_4.output( s10_signal );
      
      BitAdjust< S10_TYPE, S11_TYPE> bit_adjust( "bit_adjust", BIT_GAIN - BIT_TRIM);
      bit_adjust.clock( div_clock.div_clock); 
      bit_adjust.reset( stimulus1.reset);
      bit_adjust.input( s10_signal );
      bit_adjust.output( s11_signal );

      // Record output signal to file.
      FileRecorder< S0_TYPE, RESET_TYPE> record_s0( "record0", "input.dat" );
      record_s0.clock( stimulus.clock );
      record_s0.reset( stimulus.reset );
      record_s0.input( s0_signal );

      // Record output signal to file.
      FileRecorder< S1_TYPE, RESET_TYPE> record_s1( "record1", "out1.dat" );
      record_s1.clock( stimulus.clock );
      record_s1.reset( stimulus.reset );
      record_s1.input( s1_signal );

      // Record output signal to file.
      FileRecorder< S2_TYPE, RESET_TYPE> record_s2( "record2", "out2.dat" );
      record_s2.clock( stimulus.clock );
      record_s2.reset( stimulus.reset );
      record_s2.input( s2_signal );

      // Record output signal to file.
      FileRecorder< S3_TYPE, RESET_TYPE> record_s3( "record3", "out3.dat" );
      record_s3.clock( stimulus.clock );
      record_s3.reset( stimulus.reset );
      record_s3.input( s3_signal );

      // Record output signal to file.
      FileRecorder< S4_TYPE, RESET_TYPE> record_s4( "record4", "out4.dat" );
      record_s4.clock( stimulus.clock );
      record_s4.reset( stimulus.reset );
      record_s4.input( s4_signal );

      // Record output signal to file.
      FileRecorder< S5_TYPE, RESET_TYPE> record_s5( "record5", "out5.dat" );
      record_s5.clock( div_clock.div_clock);
      record_s5.reset( stimulus1.reset );
      record_s5.input( s5_signal );

      // Record output signal to file.
      FileRecorder< S6_TYPE, RESET_TYPE> record_s6( "record6", "out6.dat" );
      record_s6.clock( div_clock.div_clock  );
      record_s6.reset( stimulus1.reset );
      record_s6.input( s6_signal );

      // Record output signal to file.
      FileRecorder< S7_TYPE, RESET_TYPE> record_s7( "record7", "out7.dat" );
      record_s7.clock( div_clock.div_clock );
      record_s7.reset( stimulus1.reset );
      record_s7.input( s7_signal );

      // Record output signal to file.
      FileRecorder< S8_TYPE, RESET_TYPE> record_s8( "record8", "out8.dat" );
      record_s8.clock( div_clock.div_clock );
      record_s8.reset( stimulus1.reset );
      record_s8.input( s8_signal );

      // Record output signal to file.
      FileRecorder< S9_TYPE, RESET_TYPE> record_s9( "record9", "out9.dat" );
      record_s9.clock( div_clock.div_clock );
      record_s9.reset( stimulus1.reset );
      record_s9.input( s9_signal );

      // Record output signal to file.
      FileRecorder< S10_TYPE, RESET_TYPE> record_s10( "record10", "out10.dat" );
      record_s10.clock( div_clock.div_clock );
      record_s10.reset( stimulus1.reset );
      record_s10.input( s10_signal );

      // Record output signal to file.
      FileRecorder< S11_TYPE, RESET_TYPE> record_s11( "record11", "out11.dat" );
      record_s11.clock( div_clock.div_clock );
      record_s11.reset( stimulus1.reset );
      record_s11.input( s11_signal );

      //sc_trace(fout, stimulus.clock, "stim.clock");
      sc_trace ( fout , stimulus   , "stim"    ) ;
      sc_trace ( fout , stimulus1  , "stim2"   ) ;
      sc_trace ( fout , div_clock  , "clk_div" ) ;

      sc_trace ( fout , s0_signal  , "sig0"    ) ;
      sc_trace ( fout , s1_signal  , "sig1"    ) ;
      sc_trace ( fout , s2_signal  , "sig2"    ) ;
      sc_trace ( fout , s3_signal  , "sig3"    ) ;
      sc_trace ( fout , s4_signal  , "sig4"    ) ;
      sc_trace ( fout , s5_signal  , "sig5"    ) ;
      sc_trace ( fout , s6_signal  , "sig6"    ) ;
      sc_trace ( fout , s7_signal  , "sig7"    ) ;
      sc_trace ( fout , s8_signal  , "sig8"    ) ;
      sc_trace ( fout , s9_signal  , "sig9"    ) ;
      sc_trace ( fout , s10_signal  , "sig10"    ) ;
      sc_trace ( fout , s11_signal  , "sig11"    ) ;

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
