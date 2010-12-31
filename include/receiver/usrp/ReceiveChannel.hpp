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
#ifndef RECEIVE_CHANNEL_HPP
#define RECEIVE_CHANNEL_HPP

#include <boost/shared_ptr.hpp>

#include<systemc.h>

#include<sdr_simulator/SdrComplexModule.hpp>
#include<sdr_simulator/misc/PhaseAccumulator.hpp>
#include<sdr_simulator/down_converter/cordic/Cordic.hpp>
#include<sdr_simulator/receiver/usrp/FilterStage.hpp>

// locally-generated constants file
#include "configuration.hpp"

namespace usrp
{

   class ReceiveChannel:
      public sdr_module::ComplexModule< INPUT_TYPE, OUTPUT_TYPE >
   {
      typedef sc_export< sc_signal_inout_if<bool> > sc_export_clk;

      // filter stage definitions
      typedef boost::shared_ptr< usrp::FilterStage > FilterStagePtr;
      FilterStagePtr iFilter_;
      FilterStagePtr qFilter_;

      // cordic definitions
      typedef boost::shared_ptr< Cordic > CordicPtr;
      CordicPtr cordic_;

      // phase accumulator definitions
      typedef boost::shared_ptr< PhaseAccumulator > AccumulatorPtr;
      AccumulatorPtr accumulator_;

      // accumulator phase output --> cordic phase input
      sc_signal < cordic::PHASE_TYPE > phase_input_signal;

      // receive chain signal inputs --> cordic signal inputs
      sc_signal < INPUT_TYPE > real_input_signal;
      sc_signal < INPUT_TYPE > imag_input_signal;

      // cordic signal outputs --> filter signal inputs
      sc_signal < OUTPUT_TYPE > real_cordic_output_signal;
      sc_signal < OUTPUT_TYPE > imag_cordic_output_signal;
      sc_signal < cordic::PHASE_TYPE > phase_cordic_output_signal;

      // filter outputs --> receive chain outputs
      sc_signal < OUTPUT_TYPE > real_output_signal;
      sc_signal < OUTPUT_TYPE > imag_output_signal;

      // wrapper to initialize all modules
      void Initialize() {
         InitializeSignals();
         InitializeNCO();
         InitializeAccumulator();
         InitializeFilter();
      }

      // setup signal constants
      void InitializeSignals() { }

      // setup down-converter module
      void InitializeNCO() {

         cordic_ = CordicPtr ( new Cordic ( "cordic" ) );
         cordic_->clock ( this->clock );
         cordic_->reset ( this->reset );
         cordic_->real_input ( this->real_input );
         cordic_->imag_input ( this->imag_input );
         cordic_->phase_input ( phase_input_signal );
         cordic_->real_output ( real_cordic_output_signal );
         cordic_->imag_output ( imag_cordic_output_signal );
         cordic_->phase_output ( phase_cordic_output_signal );

         //TODO: DEBUG ONLY 
         debug_cordic_i_output( cordic_->debug_cordic_i_output );
         debug_cordic_q_output( cordic_->debug_cordic_q_output );

      }

      // setup phase accumulator
      void InitializeAccumulator() {
         // initialize accumulator signals
         accumulator_ = AccumulatorPtr ( new PhaseAccumulator ( "accumulator" ));
         accumulator_->reset ( this->reset );
         accumulator_->clock ( this->clock );
         accumulator_->output ( phase_input_signal );
      }

      // setup filter stages
      void InitializeFilter() {

         iFilter_ = FilterStagePtr ( new usrp::FilterStage ( "inphase_filter" ) );
         iFilter_->clock ( this->clock );
         iFilter_->reset ( this->reset );
         iFilter_->decimation ( decimation );
         iFilter_->input ( real_cordic_output_signal );
         iFilter_->output ( this->real_output );

         qFilter_ = FilterStagePtr ( new usrp::FilterStage ( "quadrature_filter" ) );
         qFilter_->clock ( this->clock );
         qFilter_->reset ( this->reset );
         qFilter_->decimation ( decimation );
         qFilter_->input ( imag_cordic_output_signal );
         qFilter_->output ( this->imag_output );

         cic_output_clock( iFilter_->cic_output_clock );
         debug_cic_i_output( iFilter_->debug_cic_output );
         debug_cic_q_output( qFilter_->debug_cic_output );
      }

      virtual void Compute() { }

      public:

      SC_HAS_PROCESS ( ReceiveChannel );

      //CTOR
      ReceiveChannel ( const sc_module_name& nm ) : 
         sdr_module::ComplexModule<INPUT_TYPE,OUTPUT_TYPE>( nm ) {

            Initialize();
            output_clock( iFilter_->div_clock );
         }

      sc_in < filter::DECIMATION_TYPE > decimation;
      sc_export_clk output_clock;
      sc_export_clk cic_output_clock;

      // TODO: Debug only - remove me
      debug::sc_export_cic_output debug_cic_i_output;
      debug::sc_export_cic_output debug_cic_q_output;
      debug::sc_export_cordic_output debug_cordic_i_output;
      debug::sc_export_cordic_output debug_cordic_q_output;

   };

}
#endif
