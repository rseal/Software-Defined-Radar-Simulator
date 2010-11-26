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
#ifndef USRP_HPP
#define USRP_HPP

#include <boost/shared_ptr.hpp>

#include<systemc.h>

#include<sdr_simulator/receiver/SdrComplexModule.hpp>
#include<sdr_simulator/misc/PhaseAccumulator.hpp>
#include<sdr_simulator/down_convert/cordic/Cordic.hpp>
#include<sdr_simulator/receiver/usrp/FilterStage.hpp>

namespace usrp
{

template < typename INPUT_DATA_TYPE, typename OUTPUT_DATA_TYPE >
class ReceiveChain:
    public sdr_module::ComplexModule< INPUT_DATA_TYPE, OUTPUT_DATA_TYPE >
{

    // filter stage definitions
    typedef boost::shared_ptr< usrp::FilterStage > FilterStagePtr;
    FilterStagePtr iFilter_;
    FilterStagePtr qFilter_;

    // cordic definitions
    typedef Cordic<CORDIC::XY_WIDTH, CORDIC::Z_WIDTH> CordicModule;
    typedef boost::shared_ptr< CordicModule > CordicPtr;
    CordicPtr cordic_;

    // phase accumulator definitions
    typedef PhaseAccumulator<ACCUMULATOR::BIT_WIDTH> AccumulatorModule;
    typedef boost::shared_ptr< AccumulatorModule > AccumulatorPtr;
    AccumulatorPtr accumulator_;

    // data type definitions
    typedef bool bit_type;
    typedef sc_int< ACCUMULATOR::BIT_WIDTH > phase_out_type;
    typedef sc_int< CORDIC::Z_WIDTH > phase_in_type;
    typedef sc_int< INPUT_WIDTH > data_input_type;
    typedef sc_int< OUTPUT_WIDTH > data_output_type;
    typedef sc_int< CIC::OUTPUT_WIDTH > cic_output_type;
    typedef sc_int< CIC::DECIMATION_WIDTH > cic_decimation_type;
    typedef sc_int< HALF_BAND_FILTER::OUTPUT_WIDTH > hbf_output_type;

    // data signal definitions
    sc_signal < phase_in_type > z_data_out_signal;
    sc_signal < phase_in_type > phase_in_signal;
    sc_signal < phase_out_type > phase_out_signal;
    sc_signal < data_input_type > y_data_in_signal;
    sc_signal < data_output_type > x_data_out_signal;
    sc_signal < data_output_type > y_data_out_signal;
    sc_signal < cic_output_type > cic_output_signal;

    // wrapper to initialize all modules
    void Initialize() {
        InitializeSignals();
        InitializeNCO();
        InitializeAccumulator();
        InitializeFilter();
    }

    // setup signal constants
    void InitializeSignals() {
        // set cordic y-input to gnd.
        y_data_in_signal.write(0);
    }

    // setup down-converter module
    void InitializeNCO() {
        cordic_ = CordicPtr(
                      new CordicModule( CORDIC::NAME, CORDIC::NUM_STAGES )
                  );
        cordic_->clock ( clock );
        cordic_->reset ( reset );
        cordic_->xin ( input );
        cordic_->yin( y_data_in_signal );
        cordic_->zin( phase_in_signal );
        cordic_->xout( x_data_out_signal );
        cordic_->yout( y_data_out_signal );
        cordic_->zout( z_data_out_signal );
    }

    // setup phase accumulator
    void InitializeAccumulator() {
        // initialize accumulator signals
        accumulator_ = AccumulatorPtr(
                           new AccumulatorModule( ACCUMULATOR::NAME, ACCUMULATOR::StepSize() )
                       );
        accumulator_->reset ( reset );
        accumulator_->clock ( clock );
        accumulator_->out ( phase_out_signal );
    }

    // setup filter stages
    void InitializeFilter() {
        iFilter_ = FilterStagePtr( new usrp::FilterStage("iFilters") );
        iFilter_->clock( clock);
        iFilter_->decimation( decimation );
        iFilter_->input( x_data_out_signal );
        iFilter_->output( i_output );
        iFilter_->reset( reset );

        qFilter_ = FilterStagePtr( new usrp::FilterStage("qFilters") );
        qFilter_->clock( clock);
        qFilter_->decimation( decimation );
        qFilter_->input( y_data_out_signal );
        qFilter_->output( q_output );
        qFilter_->reset( reset );
    }

    // computation performed on each clock cycle
    void Compute() {
        phase_out_type buffer = phase_out_signal.read();

        // verified 07/12/2010
        phase_in_signal.write(
            phase_in_type(
                buffer.range(
                    ACCUMULATOR::BIT_WIDTH-1,
                    ACCUMULATOR::BIT_WIDTH-CORDIC::Z_WIDTH
                )
            )
        );
    }

public:

    SC_HAS_PROCESS( ReceiveChain );

    //CTOR
    ReceiveChain(const sc_module_name& nm): sc_module( nm ) {

        Initialize();

        SC_METHOD( Compute );
        sensitive << clock.pos();
    }

    // define interface
    sc_in_clk clock;
    sc_in < sdr_types::reset_type > reset;
    sc_in < data_input_type > input;
    sc_in < cic_decimation_type > decimation;
    sc_out < data_output_type > i_output;
    sc_out < data_output_type > q_output;

};

}
#endif
