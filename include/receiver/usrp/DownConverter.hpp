#ifndef USRP_DOWN_CONVERTER_HPP
#define USRP_DOWN_CONVERTER_HPP

#include <boost/shared_ptr.hpp>

#include <sdr_simulator/SdrComplexModule.hpp>
#include <sdr_simulator/down_converter/cordic/Cordic.hpp>
#include <sdr_simulator/misc/PhaseAccumulator.hpp>

// locally-generated constant file.
#include "configuration.hpp"

namespace usrp
{

class DownConverter:
    public sdr_module::ComplexModule< cordic::INPUT_TYPE, cordic::OUTPUT_TYPE>
{
    // define cordic module
    typedef boost::shared_ptr< Cordic > CordicPtr;
    CordicPtr cordic_;

    // define phase accumulator module
    typedef boost::shared_ptr< PhaseAccumulator > PhaseAccumulatorPtr;
    PhaseAccumulatorPtr accumulator_;

    // internal signal definitions
    sc_signal< cordic::INPUT_TYPE > real_din_signal;
    sc_signal< cordic::INPUT_TYPE > real_dout_signal;
    sc_signal< cordic::INPUT_TYPE > imag_din_signal;
    sc_signal< cordic::INPUT_TYPE > imag_dout_signal;
    sc_signal< cordic::PHASE_TYPE > phase_din_signal;
    sc_signal< cordic::PHASE_TYPE > phase_dout_signal;

    // operates on positive clock edge.
    virtual void Compute()
    { 
       if( !this->reset.read() )
       {
          this->real_output.write( real_dout_signal.read() );
          this->imag_output.write( imag_dout_signal.read() );
          this->phase_output.write( phase_dout_signal.read() );
       }
    
    }

public:
    DownConverter ( const sc_module_name& name ) :
        sdr_module::ComplexModule< cordic::INPUT_TYPE, cordic::OUTPUT_TYPE> ( name ) {

           // create phase accumulator
           accumulator_ = PhaseAccumulatorPtr( new PhaseAccumulator("accumulator") );
           accumulator_->clock ( this->clock );
           accumulator_->reset ( this->reset );
           accumulator_->output ( phase_din_signal );

           // create cordic module
           cordic_ = CordicPtr( new Cordic("cordic") );
           cordic_->reset ( this->reset );
           cordic_->clock ( this->clock );
           cordic_->phase_input( phase_din_signal );
           cordic_->phase_output( phase_dout_signal );
           cordic_->real_input( this->real_input );
           cordic_->imag_input( this->imag_input );
           cordic_->real_output( real_dout_signal );
           cordic_->imag_output( imag_dout_signal );

        }

    sc_out< cordic::PHASE_TYPE > phase_output;

};
}



#endif
