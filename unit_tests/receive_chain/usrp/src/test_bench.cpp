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
#include <sdr_simulator/util/SinusoidGenerator.hpp>
#include <sdr_simulator/util/GaussianNoiseGenerator.hpp>
#include <sdr_simulator/util/FileRecorder.hpp>
#include <sdr_simulator/util/PulseGenerator.hpp>

#include "configuration.hpp"

using namespace usrp;

int sc_main ( int argc, char* argv[] )
{
    // TODO: Move to header generator
    const double TIME_RESOLUTION = 100.0;
    const double CLOCK_PERIOD = 1.0e9 / SAMPLE_RATE;
    const int RESET_TIME = 10;
    const double TOTAL_SIMULATION_TIME = 1e5*CLOCK_PERIOD;

    const string I_DATA_FILE_NAME = "i_out.dat";
    const string Q_DATA_FILE_NAME = "q_out.dat";

    // set time parameters
    sc_set_time_resolution ( TIME_RESOLUTION , SC_PS );
    sc_time clock_time ( CLOCK_PERIOD, SC_NS );
    sc_time simulation_time ( TOTAL_SIMULATION_TIME,SC_NS );

    // interface signals
    sc_signal<RESET_TYPE> reset_signal;

    // signals from signal generator to receive chain input
    sc_signal<INPUT_TYPE> i_in_signal;
    sc_signal<INPUT_TYPE> q_in_signal;

    // signals from receive chain output to file recorder
    sc_signal<OUTPUT_TYPE> i_out_signal;
    sc_signal<OUTPUT_TYPE> q_out_signal;

    sc_signal< DECIMATION_TYPE > decimation_signal;

    Stimulus< RESET_TYPE > stimulus ( "stimulus", clock_time, RESET_TIME );

    // the y-input to the CORDIC is constant. This will
    // peform down-conversion and provide a complex output.
    q_in_signal.write ( 0 );

    // set decimation
    decimation_signal.write ( usrp::DECIMATION );

    double sample_rate = 64e6;
    double ipp_width = 64e5;
    double doppler_shift = 15e3;
    double samples_per_usec = 1e-6*sample_rate;
    double pulse_width = 2.0*samples_per_usec;
    double rf_carrier = 49.8e6;
    double frequency = rf_carrier + doppler_shift;
    double normalized_frequency = frequency / sample_rate ;

//    PulseGenerator< usrp::OUTPUT_TYPE, usrp::RESET_TYPE> 
//       signal_generator( "pulse_gen", pulse_width, ipp_width, normalized_frequency , 16383*0.95);
//    signal_generator.output( i_in_signal );
//    signal_generator.reset( stimulus.reset );
//    signal_generator.clock( stimulus.clock );

    //GaussianNoiseGenerator< usrp::OUTPUT_TYPE, usrp::RESET_TYPE> 
       //noise_generator( "noise_gen", 0.0, 1.0, 0.10);
    //noise_generator.output( i_in_signal );
    //noise_generator.reset( stimulus.reset );
    //noise_generator.clock( stimulus.clock );

    // generate a sinusoid to feed into the x-input for testing.
    SinusoidGenerator<usrp::INPUT_TYPE, usrp::RESET_TYPE >
      iSigGen ( "i_signal_gen", normalized_frequency, 12, 0.75 );
    iSigGen.reset ( stimulus.reset );
    iSigGen.clock ( stimulus.clock );
    iSigGen.output ( i_in_signal );

    usrp::ReceiveChannel dut ( "receive_channel" );
    dut.reset ( stimulus.reset );
    dut.clock ( stimulus.clock );
    dut.real_input ( i_in_signal );
    dut.imag_input ( q_in_signal );
    dut.real_output ( i_out_signal );
    dut.imag_output ( q_out_signal );
    dut.decimation ( decimation_signal );

    // Store the x data output into a file.
    FileRecorder< OUTPUT_TYPE, RESET_TYPE >
       iInputRecorder ( "i_input", "input.dat");
    iInputRecorder.clock ( stimulus.clock );
    iInputRecorder.reset ( stimulus.reset );
    iInputRecorder.input ( i_in_signal );

    // TODO: Debug only 
    FileRecorder< OUTPUT_TYPE, RESET_TYPE >
       cic_i_DebugRecorder ( "debug_cic_i_output", "cic_i_output.dat");
    cic_i_DebugRecorder.clock ( dut.cic_output_clock );
    cic_i_DebugRecorder.reset ( stimulus.reset );
    cic_i_DebugRecorder.input ( dut.debug_cic_i_output );

    // TODO: Debug only 
    FileRecorder< OUTPUT_TYPE, RESET_TYPE >
       cic_q_DebugRecorder ( "debug_cic_q_output", "cic_q_output.dat");
    cic_q_DebugRecorder.clock ( dut.cic_output_clock );
    cic_q_DebugRecorder.reset ( stimulus.reset );
    cic_q_DebugRecorder.input ( dut.debug_cic_q_output );

    // TODO: Debug only 
    FileRecorder< OUTPUT_TYPE, RESET_TYPE >
       cordic_i_DebugRecorder ( "debug_cordic_i_output", "cordic_i_output.dat");
    cordic_i_DebugRecorder.clock ( stimulus.clock );
    cordic_i_DebugRecorder.reset ( stimulus.reset );
    cordic_i_DebugRecorder.input ( dut.debug_cordic_i_output );

    // TODO: Debug only 
    FileRecorder< OUTPUT_TYPE, RESET_TYPE >
       cordic_q_DebugRecorder ( "debug_cordic_q_output", "cordic_q_output.dat");
    cordic_q_DebugRecorder.clock ( stimulus.clock );
    cordic_q_DebugRecorder.reset ( stimulus.reset );
    cordic_q_DebugRecorder.input ( dut.debug_cordic_q_output );

    // Store the x data output into a file.
    FileRecorder< OUTPUT_TYPE, RESET_TYPE >
       iDataRecorder ( "i_data_recorder", I_DATA_FILE_NAME );
    iDataRecorder.clock ( dut.output_clock );
    iDataRecorder.reset ( stimulus.reset );
    iDataRecorder.input ( i_out_signal );

    // Store the y data output into a file.
    FileRecorder< OUTPUT_TYPE, RESET_TYPE >
       qDataRecorder ( "q_data_recorder", Q_DATA_FILE_NAME );
    qDataRecorder.clock ( dut.output_clock );
    qDataRecorder.reset ( stimulus.reset );
    qDataRecorder.input ( q_out_signal );

    // begin simulation
    sc_start ( simulation_time );

    return 0;
}
