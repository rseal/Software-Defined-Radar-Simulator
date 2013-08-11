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
#include <fstream>

#include <sdr_simulator/sampler/Adc.hpp>
#include <sdr_simulator/filter/fir/AnalogFirFilter.hpp>
#include <sdr_simulator/input/Stimulus.hpp>
#include <sdr_simulator/input/AnalogSinusoidGenerator.hpp>
#include <sdr_simulator/input/GaussianNoiseGenerator.hpp>
#include <sdr_simulator/output/FileRecorder.hpp>
#include <sdr_simulator/input/PulseGenerator.hpp>
#include <sdr_simulator/yaml/NodeParser.hpp>
#include <sdr_simulator/yaml/AdcYaml.hpp>

#include "ReceiveChannel.hpp"
#include "configuration.hpp"

using namespace usrp_v1;

int sc_main ( int argc, char* argv[] )
{
   const std::string STIMULUS_FILE = "stimulus.yml";

   try
   {

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
      const double PRI              = root["pri"].as<double>();
      const double SIGNAL_FREQ      = root["signal_frequency"].as<double>();
      const double AMPLITUDE        = root["amplitude"].as<double>();
      const double DOPPLER          = root["doppler"].as<double>();
      const double MEAN             = root["mean"].as<double>();
      const double VAR              = root["variance"].as<double>();
      const double VFS              = root["adc_vfs"].as<double>();
      const double SNR              = root["adc_snr"].as<double>();
      const int    ADC_WIDTH        = root["adc_width"].as<int>();
      const double BANDWIDTH        = root["adc_bw"].as<double>();
      const double DDC_FREQUENCY    = root["ddc_frequency"].as<double>();
      const string OUTPUT_DATA_FILE = root["output_file_name"].as<string>();
      const string INPUT_DATA_FILE  = root["input_file_name"].as<string>();
      const double SCALE                 = tr1::pow(2.0, accumulator::ACC_WIDTH);
      const int STEP_SIZE                = rint( SCALE*DDC_FREQUENCY/SAMPLE_RATE );

      cout 
         << "PULSE_WIDTH : " << PULSE_WIDTH << "\n"
         << "PRI         : " << PRI << "\n"
         << "SIGNAL_FREQ : " << SIGNAL_FREQ << "\n"
         << "AMPLITUDE   : " << AMPLITUDE << "\n"
         << "DOPPLER     : " << DOPPLER << "\n"
         << "VFS         : " << VFS << "\n"
         << "SNR         : " << SNR << "\n"
         << "ADC_WIDTH   : " << ADC_WIDTH << "\n"
         << "BANDWIDTH   : " << BANDWIDTH << "\n"
         << "STEP_SIZE   : " << STEP_SIZE << "\n"
         << endl;
      //double frequency = pg_data.rfFrequency + pg_data.doppler;
      //double voltage_pp = 1.0;

      // set time parameters
      sc_set_time_resolution  ( TIME_RESOLUTION, SC_PS ) ;
      sc_time clock_time      ( CLOCK_PERIOD, SC_NS ) ;
      sc_time simulation_time ( TOTAL_SIMULATION_TIME, SC_NS ) ;

      // interface signals
      sc_signal<sdr::RESET_TYPE> reset_signal;

      // signals from signal generator to receive chain input
      sc_signal<double> input_signal;
      sc_signal<INPUT_TYPE> null_signal;
      sc_signal<INPUT_TYPE> adc_signal;
      sc_signal<INPUT_TYPE> fir_signal;

      // signals from receive chain output to file recorder
      sc_signal<OUTPUT_TYPE> i_out_signal;
      sc_signal<OUTPUT_TYPE> q_out_signal;

      sc_signal< sc_int<16> > decimation_signal;

      /////////////////////////////////////////////////////////////////////////////
      // STAGE 0 : Stimulus Generator
      /////////////////////////////////////////////////////////////////////////////
      Stimulus< sdr::RESET_TYPE > stimulus ( "stimulus", clock_time, RESET_TIME);

      // the y-input to the CORDIC is constant. This will
      // peform down-conversion and provide a complex output.
      null_signal.write ( 0 );

      // set decimation
      decimation_signal.write ( DECIMATION );

      /////////////////////////////////////////////////////////////////////////////
      // STAGE 1 : Analog Signal Generation
      /////////////////////////////////////////////////////////////////////////////
      AnalogSinusoidGenerator<double,sdr::RESET_TYPE>
         signal_generator("sin_gen", ( SIGNAL_FREQ + DOPPLER)/SAMPLE_RATE, ADC_WIDTH); 
      signal_generator.output( input_signal );
      signal_generator.reset( stimulus.reset );
      signal_generator.clock( stimulus.clock );


      /////////////////////////////////////////////////////////////////////////////
      // STAGE 2 : Analog-to-Digital Conversion
      /////////////////////////////////////////////////////////////////////////////
      Adc< OUTPUT_TYPE> 
         sampler( "sampler", VFS, SAMPLE_RATE, SNR, BANDWIDTH, ADC_WIDTH);
      sampler.input( input_signal );
      sampler.reset( stimulus.reset );
      sampler.clock( stimulus.clock );
      sampler.output( adc_signal );

      typedef AnalogFirFilter< INPUT_TYPE, OUTPUT_TYPE> Filter;

      Filter filter("filter");
      Filter::CoeffVector coefficients;
      coefficients.push_back(2.08295393e-03);
      coefficients.push_back(3.86002797e-03);
      coefficients.push_back(4.86941328e-03);
      coefficients.push_back(3.14234421e-03);
      coefficients.push_back(-3.23738725e-03);
      coefficients.push_back(-1.31304870e-02);
      coefficients.push_back(-2.09128528e-02);
      coefficients.push_back(-1.89755071e-02);
      coefficients.push_back(-3.38737483e-03);
      coefficients.push_back(2.12711783e-02);
      coefficients.push_back(4.23150498e-02);
      coefficients.push_back(4.57137146e-02);
      coefficients.push_back(2.54427033e-02);
      coefficients.push_back(-1.10212391e-02);
      coefficients.push_back(-4.55753411e-02);
      coefficients.push_back(9.35085607e-01);
      coefficients.push_back(-4.55753411e-02);
      coefficients.push_back(-1.10212391e-02);
      coefficients.push_back(2.54427033e-02);
      coefficients.push_back(4.57137146e-02);
      coefficients.push_back(4.23150498e-02);
      coefficients.push_back(2.12711783e-02);
      coefficients.push_back(-3.38737483e-03);
      coefficients.push_back(-1.89755071e-02);
      coefficients.push_back(-2.09128528e-02);
      coefficients.push_back(-1.31304870e-02);
      coefficients.push_back(-3.23738725e-03);
      coefficients.push_back(3.14234421e-03);
      coefficients.push_back(4.86941328e-03);
      coefficients.push_back(3.86002797e-03);
      coefficients.push_back(2.08295393e-03);

      filter.LoadCoefficients( coefficients );
      filter.input( adc_signal );
      filter.reset( stimulus.reset );
      filter.clock( stimulus.clock );
      filter.output( fir_signal );


      /////////////////////////////////////////////////////////////////////////////
      // STAGE 3 : Receiver Channel
      /////////////////////////////////////////////////////////////////////////////
      usrp_v1::ReceiveChannel dut ( "receive_channel" );
      dut.reset ( stimulus.reset );
      dut.clock ( stimulus.clock );
      dut.real_input ( fir_signal );
      dut.imag_input ( null_signal );
      dut.real_output ( i_out_signal );
      dut.imag_output ( q_out_signal );
      dut.decimation ( decimation_signal );
      dut.StepSize( STEP_SIZE );

      /////////////////////////////////////////////////////////////////////////////
      // RECORDER : Record in-phase input signal
      /////////////////////////////////////////////////////////////////////////////
      FileRecorder< double, sdr::RESET_TYPE >
         iInputRecorder ( "i_input", "input.dat");
      iInputRecorder.clock ( stimulus.clock );
      iInputRecorder.reset ( stimulus.reset );
      iInputRecorder.input ( input_signal );

      /////////////////////////////////////////////////////////////////////////////
      // RECORDER : Record in-phase input signal
      /////////////////////////////////////////////////////////////////////////////
      FileRecorder< OUTPUT_TYPE, sdr::RESET_TYPE >
         adcRecorder ( "adc_output", "adc.dat");
      adcRecorder.clock ( stimulus.clock );
      adcRecorder.reset ( stimulus.reset );
      adcRecorder.input ( adc_signal );

      /////////////////////////////////////////////////////////////////////////////
      // RECORDER : Record in-phase input signal
      /////////////////////////////////////////////////////////////////////////////
      FileRecorder< OUTPUT_TYPE, sdr::RESET_TYPE >
         firRecorder ( "fir_output", "fir.dat");
      firRecorder.clock ( stimulus.clock );
      firRecorder.reset ( stimulus.reset );
      firRecorder.input ( fir_signal );

      /////////////////////////////////////////////////////////////////////////////
      // RECORDER : Record CIC in-phase output signal
      /////////////////////////////////////////////////////////////////////////////
      FileRecorder< OUTPUT_TYPE, sdr::RESET_TYPE >
         cordic_i_recorder ( "cordic_i_output", "cordic_i_output.dat");
      cordic_i_recorder.clock ( dut.clock );
      cordic_i_recorder.reset ( stimulus.reset );
      cordic_i_recorder.input ( dut.cordic_i_out);

      /////////////////////////////////////////////////////////////////////////////
      // RECORDER : Record CIC in-phase output signal
      /////////////////////////////////////////////////////////////////////////////
      FileRecorder< OUTPUT_TYPE, sdr::RESET_TYPE >
         cordic_q_recorder ( "cordic_q_output", "cordic_q_output.dat");
      cordic_q_recorder.clock ( dut.clock );
      cordic_q_recorder.reset ( stimulus.reset );
      cordic_q_recorder.input ( dut.cordic_q_out);

      /////////////////////////////////////////////////////////////////////////////
      // RECORDER : Record CIC quadrature input signal
      /////////////////////////////////////////////////////////////////////////////
      //FileRecorder< OUTPUT_TYPE, sdr::RESET_TYPE >
      //cic_q_DebugRecorder ( "debug_cic_q_output", "cic_q_output.dat");
      //cic_q_DebugRecorder.clock ( dut.cic_output_clock );
      //cic_q_DebugRecorder.reset ( stimulus.reset );
      //cic_q_DebugRecorder.input ( dut.debug_cic_q_output );

      /////////////////////////////////////////////////////////////////////////////
      // RECORDER : Record CIC in-phase output signal
      /////////////////////////////////////////////////////////////////////////////
      //FileRecorder< OUTPUT_TYPE, sdr::RESET_TYPE >
      //cordic_i_DebugRecorder ( "debug_cordic_i_output", "cordic_i_output.dat");
      //cordic_i_DebugRecorder.clock ( stimulus.clock );
      //cordic_i_DebugRecorder.reset ( stimulus.reset );
      //cordic_i_DebugRecorder.input ( dut.debug_cordic_i_output );

      /////////////////////////////////////////////////////////////////////////////
      // RECORDER : Record CIC quadrature output signal
      /////////////////////////////////////////////////////////////////////////////
      //FileRecorder< OUTPUT_TYPE, sdr::RESET_TYPE >
      //cordic_q_DebugRecorder ( "debug_cordic_q_output", "cordic_q_output.dat");
      //cordic_q_DebugRecorder.clock ( stimulus.clock );
      //cordic_q_DebugRecorder.reset ( stimulus.reset );
      //cordic_q_DebugRecorder.input ( dut.debug_cordic_q_output );

      /////////////////////////////////////////////////////////////////////////////
      // RECORDER : Record in-phase output signal
      /////////////////////////////////////////////////////////////////////////////
      FileRecorder< OUTPUT_TYPE, sdr::RESET_TYPE >
         iDataRecorder ( "i_data_recorder", "i_out.dat" );
      iDataRecorder.clock ( dut.output_clock );
      iDataRecorder.reset ( stimulus.reset );
      iDataRecorder.input ( i_out_signal );

      /////////////////////////////////////////////////////////////////////////////
      // RECORDER : Record quadrature output signal
      /////////////////////////////////////////////////////////////////////////////
      FileRecorder< OUTPUT_TYPE, sdr::RESET_TYPE >
         qDataRecorder ( "q_data_recorder", "q_out.dat" );
      qDataRecorder.clock ( dut.output_clock );
      qDataRecorder.reset ( stimulus.reset );
      qDataRecorder.input ( q_out_signal );

      // begin simulation
      sc_start ( simulation_time );

   }
   catch ( YAML::ParserException& e )
   {
      std::cout << e.what();
   }

   return EXIT_SUCCESS;
}
