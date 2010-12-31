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
#include <sdr_simulator/receiver/usrp/DownConverter.hpp>
#include <sdr_simulator/util/Stimulus.hpp>
#include <sdr_simulator/util/SinusoidGenerator.hpp>
#include <sdr_simulator/util/FileRecorder.hpp>

#include "configuration.hpp"

using namespace cordic;

int sc_main ( int argc, char* argv[] )
{
    // TODO: Move to header generator
    const double TIME_RESOLUTION = 100.0;
    const double CLOCK_PERIOD = 1.0e9 / SAMPLE_RATE;
    const int RESET_TIME = 10;
    const double TOTAL_SIMULATION_TIME = 1e6*CLOCK_PERIOD;
    const string X_DATA_FILE_NAME = "xout.dat";
    const string Y_DATA_FILE_NAME = "yout.dat";
    const string Z_DATA_FILE_NAME = "zout.dat";
    const string INPUT_SIGNAL_FILE_NAME = "inputSignal.dat";

    // set time parameters
    sc_set_time_resolution ( TIME_RESOLUTION , SC_PS );
    sc_time clock_time ( CLOCK_PERIOD, SC_NS );
    sc_time simulation_time ( TOTAL_SIMULATION_TIME,SC_NS );

    // interface signals
    sc_signal<RESET_TYPE> reset_signal;
    sc_signal<OUTPUT_TYPE> x_data_signal;
    sc_signal<OUTPUT_TYPE> y_data_signal;
    sc_signal<PHASE_TYPE> phase_data_signal;
    sc_signal<OUTPUT_TYPE> x_data_output_signal;
    sc_signal<OUTPUT_TYPE> y_data_output_signal;
    sc_signal<PHASE_TYPE> z_data_output_signal;

    Stimulus< RESET_TYPE > stimulus ( "stimulus", clock_time, RESET_TIME );

    // the y-input to the CORDIC is constant. This will
    // peform down-conversion and provide a complex output.
    y_data_signal.write ( 0 );

    // generate a sinusoid to feed into the x-input for testing.
    SinusoidGenerator<OUTPUT_TYPE, RESET_TYPE >
    sineGenerator ( "sine_generator", NORMALIZED_FREQUENCY, ADC_WIDTH, 0.9 );
    sineGenerator.reset ( stimulus.reset );
    sineGenerator.clock ( stimulus.clock );
    sineGenerator.output ( x_data_signal );

    // Store the input signal into a file.
    FileRecorder< OUTPUT_TYPE, RESET_TYPE > inputSignalRecorder (
        "input_signal_data_recorder", INPUT_SIGNAL_FILE_NAME );
    inputSignalRecorder.clock ( stimulus.clock );
    inputSignalRecorder.reset ( stimulus.reset );
    inputSignalRecorder.input ( x_data_signal );

    usrp::DownConverter dut( "down_converter" );
    dut.real_input ( x_data_signal );
    dut.imag_input ( y_data_signal );
    dut.real_output ( x_data_output_signal );
    dut.imag_output ( y_data_output_signal );
    dut.reset ( stimulus.reset );
    dut.phase_output ( z_data_output_signal );
    dut.clock ( stimulus.clock );

    // Store the x data output into a file.
    FileRecorder< OUTPUT_TYPE, RESET_TYPE > xDataRecorder (
        "x_data_recorder", X_DATA_FILE_NAME );
    xDataRecorder.clock ( stimulus.clock );
    xDataRecorder.reset ( stimulus.reset );
    xDataRecorder.input ( x_data_output_signal );

    // Store the y data output into a file.
    FileRecorder< OUTPUT_TYPE, RESET_TYPE > yDataRecorder (
        "y_data_recorder", Y_DATA_FILE_NAME );
    yDataRecorder.clock ( stimulus.clock );
    yDataRecorder.reset ( stimulus.reset );
    yDataRecorder.input ( y_data_output_signal );

    // Store the z data output into a file.
    FileRecorder< PHASE_TYPE, RESET_TYPE > zDataRecorder (
        "z_data_recorder", Z_DATA_FILE_NAME );
    zDataRecorder.clock ( stimulus.clock );
    zDataRecorder.reset ( stimulus.reset );
    zDataRecorder.input ( z_data_output_signal );


    // begin simulation
    sc_start ( simulation_time );

    return 0;
}
