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
#include <sdr_simulator/receiver/usrp/ReceiveChannel.hpp>
#include <sdr_simulator/util/Stimulus.hpp>
#include <sdr_simulator/sampler/Adc.hpp>
#include <sdr_simulator/util/SinusoidGenerator.hpp>
#include <sdr_simulator/util/GaussianNoiseGenerator.hpp>
#include <sdr_simulator/util/FileRecorder.hpp>
#include <sdr_simulator/util/PulseGenerator.hpp>
#include <sdr_simulator/yaml/NodeParser.hpp>
#include <sdr_simulator/yaml/TestBenchYaml.hpp>
#include <sdr_simulator/yaml/AdcYaml.hpp>

#include "PulseGeneratorYaml.hpp"
#include "configuration.hpp"

#include <fstream>

using namespace usrp;

int sc_main ( int argc, char* argv[] )
{

   // open configuration file and parse
   std::ifstream fin(argv[1]);
   YAML::Parser parser(fin);
   YAML::Node doc;
   parser.GetNextDocument(doc);

   // parse nodes and populate structure
   yaml::PulseGeneratorYaml pg_data;
   yaml::TestBenchYaml tb_data;
	yaml::AdcYaml adc_data;

   try
   {
      const YAML::Node& pulse_gen_node = yaml::ParseNode( doc, "pulse_generator");
      pulse_gen_node >> pg_data;
		const YAML::Node& adc_node = yaml::ParseNode( doc, "adc");
		adc_node >> adc_data;
      const YAML::Node& tb_node = yaml::ParseNode( doc, "test_bench");
      tb_node >> tb_data;
   }
   catch ( YAML::ParserException& e )
   {
      std::cout << e.what();
   }

   double frequency = pg_data.rfFrequency + pg_data.doppler;
	double voltage_pp = 1.0;

   // set time parameters
   sc_set_time_resolution  ( tb_data.timeResolution , SC_PS ) ;
   sc_time clock_time      ( tb_data.clockPeriod    , SC_NS ) ;
   sc_time simulation_time ( tb_data.simulationTime , SC_NS ) ;

   // interface signals
   sc_signal<RESET_TYPE> reset_signal;

   // signals from signal generator to receive chain input
   sc_signal<double> input_signal;
   sc_signal<INPUT_TYPE> null_signal;
   sc_signal<INPUT_TYPE> adc_signal;

   // signals from receive chain output to file recorder
   sc_signal<OUTPUT_TYPE> i_out_signal;
   sc_signal<OUTPUT_TYPE> q_out_signal;

   sc_signal< DECIMATION_TYPE > decimation_signal;

	/////////////////////////////////////////////////////////////////////////////
	// STAGE 0 : Stimulus Generator
	/////////////////////////////////////////////////////////////////////////////
   Stimulus< RESET_TYPE > stimulus ( "stimulus", clock_time, tb_data.resetTime );

   // the y-input to the CORDIC is constant. This will
   // peform down-conversion and provide a complex output.
   null_signal.write ( 0 );

   // set decimation
   decimation_signal.write ( usrp::DECIMATION );

	/////////////////////////////////////////////////////////////////////////////
	// STAGE 1 : Analog Signal Generation
	/////////////////////////////////////////////////////////////////////////////
   PulseGenerator<usrp::RESET_TYPE> 
		signal_generator( 
				"pulse_gen",
				pg_data.pulseWidth, 
				pg_data.pri, 
				accumulator::SAMPLE_RATE, 
				frequency , 
				voltage_pp
				);
   signal_generator.output( input_signal );
   signal_generator.reset( stimulus.reset );
   signal_generator.clock( stimulus.clock );

	/////////////////////////////////////////////////////////////////////////////
	// STAGE 2 : Analog-to-Digital Conversion
	/////////////////////////////////////////////////////////////////////////////
	Adc< usrp::OUTPUT_TYPE, usrp::ADC_WIDTH > 
		sampler(
				"sampler", 
				adc_data.vfs, 
				accumulator::SAMPLE_RATE,
				adc_data.snr,
				adc_data.bw
				);
	sampler.input( input_signal );
	sampler.reset( stimulus.reset );
	sampler.clock( stimulus.clock );
	sampler.output( adc_signal );

	/////////////////////////////////////////////////////////////////////////////
	// STAGE 3 : Receiver Channel
	/////////////////////////////////////////////////////////////////////////////
	usrp::ReceiveChannel dut ( "receive_channel" );
	dut.reset ( stimulus.reset );
	dut.clock ( stimulus.clock );
	dut.real_input ( adc_signal );
	dut.imag_input ( null_signal );
	dut.real_output ( i_out_signal );
	dut.imag_output ( q_out_signal );
	dut.decimation ( decimation_signal );

	/////////////////////////////////////////////////////////////////////////////
	// RECORDER : Record in-phase input signal
	/////////////////////////////////////////////////////////////////////////////
	FileRecorder< OUTPUT_TYPE, RESET_TYPE >
		iInputRecorder ( "i_input", "input.dat");
	iInputRecorder.clock ( stimulus.clock );
	iInputRecorder.reset ( stimulus.reset );
	iInputRecorder.input ( adc_signal );

	/////////////////////////////////////////////////////////////////////////////
	// RECORDER : Record CIC in-phase output signal
	/////////////////////////////////////////////////////////////////////////////
	FileRecorder< OUTPUT_TYPE, RESET_TYPE >
		cic_i_DebugRecorder ( "debug_cic_i_output", "cic_i_output.dat");
	cic_i_DebugRecorder.clock ( dut.cic_output_clock );
	cic_i_DebugRecorder.reset ( stimulus.reset );
	cic_i_DebugRecorder.input ( dut.debug_cic_i_output );

	/////////////////////////////////////////////////////////////////////////////
	// RECORDER : Record CIC quadrature input signal
	/////////////////////////////////////////////////////////////////////////////
	FileRecorder< OUTPUT_TYPE, RESET_TYPE >
		cic_q_DebugRecorder ( "debug_cic_q_output", "cic_q_output.dat");
	cic_q_DebugRecorder.clock ( dut.cic_output_clock );
	cic_q_DebugRecorder.reset ( stimulus.reset );
	cic_q_DebugRecorder.input ( dut.debug_cic_q_output );

	/////////////////////////////////////////////////////////////////////////////
	// RECORDER : Record CIC in-phase output signal
	/////////////////////////////////////////////////////////////////////////////
	FileRecorder< OUTPUT_TYPE, RESET_TYPE >
		cordic_i_DebugRecorder ( "debug_cordic_i_output", "cordic_i_output.dat");
	cordic_i_DebugRecorder.clock ( stimulus.clock );
	cordic_i_DebugRecorder.reset ( stimulus.reset );
	cordic_i_DebugRecorder.input ( dut.debug_cordic_i_output );

	/////////////////////////////////////////////////////////////////////////////
	// RECORDER : Record CIC quadrature output signal
	/////////////////////////////////////////////////////////////////////////////
	FileRecorder< OUTPUT_TYPE, RESET_TYPE >
		cordic_q_DebugRecorder ( "debug_cordic_q_output", "cordic_q_output.dat");
	cordic_q_DebugRecorder.clock ( stimulus.clock );
	cordic_q_DebugRecorder.reset ( stimulus.reset );
	cordic_q_DebugRecorder.input ( dut.debug_cordic_q_output );

	/////////////////////////////////////////////////////////////////////////////
	// RECORDER : Record in-phase output signal
	/////////////////////////////////////////////////////////////////////////////
	FileRecorder< OUTPUT_TYPE, RESET_TYPE >
		iDataRecorder ( "i_data_recorder", "i_out.dat" );
	iDataRecorder.clock ( dut.output_clock );
	iDataRecorder.reset ( stimulus.reset );
	iDataRecorder.input ( i_out_signal );

	/////////////////////////////////////////////////////////////////////////////
	// RECORDER : Record quadrature output signal
	/////////////////////////////////////////////////////////////////////////////
	FileRecorder< OUTPUT_TYPE, RESET_TYPE >
		qDataRecorder ( "q_data_recorder", "q_out.dat" );
	qDataRecorder.clock ( dut.output_clock );
	qDataRecorder.reset ( stimulus.reset );
	qDataRecorder.input ( q_out_signal );

	// begin simulation
	sc_start ( simulation_time );

	return 0;
}
