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
#include<systemc.h>
#include<iostream>


#include<boost/shared_ptr.hpp>
#include<sdr_simulator/CicIntegrator.hpp>
#include<sdr_simulator/CicDifferentiator.hpp>

template< unsigned int INPUT_WIDTH, unsigned int OUTPUT_WIDTH >
class CIC : public sc_module {

   sc_signal< sc_int< 52 > > sig_0_;
   sc_signal< sc_int< 43 > > sig_1_;
   sc_signal< sc_int< 34 > > sig_2_;
   sc_signal< sc_int< 25 > > sig_3_;
   sc_signal< sc_int< 24 > > sig_4_;
   sc_signal< sc_int< 23 > > sig_5_;
   sc_signal< sc_int< 22 > > sig_6_;
   sc_signal< sc_int< 21 > > sig_7_;
   sc_signal< sc_int< 16 > > sig_out_;

   boost::shared_ptr< CicIntegrator< 52, 43 > > integrator_0_;
   boost::shared_ptr< CicIntegrator< 43, 34 > > integrator_1_;
   boost::shared_ptr< CicIntegrator< 34, 25 > > integrator_2_;
   boost::shared_ptr< CicIntegrator< 25, 24 > > integrator_3_;
   boost::shared_ptr< CicDifferentiator< 24, 23 > > differentiator_0_;
   boost::shared_ptr< CicDifferentiator< 23, 22 > > differentiator_1_;
   boost::shared_ptr< CicDifferentiator< 22, 21 > > differentiator_2_;
   boost::shared_ptr< CicDifferentiator< 21, 16 > > differentiator_3_;

   void Compute(){
      sig_0_ = input.read();
      output = sig_out_.read();
   };


   typedef sc_uint<1> bit_type;
   typedef sc_int< INPUT_WIDTH > data_input_type;
   typedef sc_int< OUTPUT_WIDTH > data_output_type;


   public:

   SC_HAS_PROCESS( CIC );

   CIC( const sc_module_name& nm) : sc_module(nm){

      SC_METHOD( Compute );
      sensitive << clock.pos();

      integrator_0_ = new CICIntegrator< 52, 43 >( " integrator_0_" );
      integrator_0->clock( clock );
      integrator_0->reset( reset );
      integrator_0->input( sig_0_ );
      integrator_0->output( sig_1_ );

      integrator_1_ = new CICIntegrator< 43, 34 >( "integrator_1_" );
      integrator_1_->clock( clock );
      integrator_1_->reset( reset );
      integrator_1_->input( sig_1_ );
      integrator_1_->output( sig_2_ );

      integrator_2_ = new CICIntegrator< 34, 25 >( "integrator_2_" );
      integrator_2_->clock( clock );
      integrator_2_->reset( reset );
      integrator_2_->input( sig_2_ );
      integrator_2_->output( sig_3_ );

      integrator_3_ = new CICIntegrator< 25, 24 >( "integrator_3_" );
      integrator_3_->clock( clock );
      integrator_3_->reset( reset );
      integrator_3_->input( sig_3_ );
      integrator_3_->output( sig_4_ );

      differentiator_0_ = new CICDifferentiator< 24, 23 >( "differentiator_0_" );
      differentiator_0_->clock( clock );
      differentiator_0_->reset( reset );
      differentiator_0_->input( sig_4_ );
      differentiator_0_->output( sig_5 );

      differentiator_1_ = new CICDifferentiator< 23, 22 >( "differentiator_1_" );
      differentiator_1_->clock( clock );
      differentiator_1_->reset( reset );
      differentiator_1_->input( sig_5_ );
      differentiator_1_->output( sig_6 );

      differentiator_2_ = new CICDifferentiator< 22, 21 >( "differentiator_2_" );
      differentiator_2_->clock( clock );
      differentiator_2_->reset( reset );
      differentiator_2_->input( sig_6_ );
      differentiator_2_->output( sig_out_ );

   };

   sc_in_clk clock;
   sc_in< bit_type > reset;
   sc_in< data_input_type > input;
   sc_out< data_output_type > output;
};
