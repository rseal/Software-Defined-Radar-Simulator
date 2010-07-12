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
class Cic : public sc_module {

   sc_signal< sc_int< 52 > > sig_0_;
   sc_signal< sc_int< 43 > > sig_1_;
   sc_signal< sc_int< 34 > > sig_2_;
   sc_signal< sc_int< 25 > > sig_3_;
   sc_signal< sc_int< 24 > > sig_4_;
   sc_signal< sc_int< 23 > > sig_5_;
   sc_signal< sc_int< 22 > > sig_6_;
   sc_signal< sc_int< 21 > > sig_7_;
   sc_signal< sc_int< 20 > > sig_8_;

   sc_signal< bool > div_clock;
   int idx_;

   boost::shared_ptr<CicIntegrator< 52, 43 > > integrator_0_;
   boost::shared_ptr<CicIntegrator< 43, 34 > > integrator_1_;
   boost::shared_ptr<CicIntegrator< 34, 25 > > integrator_2_;
   boost::shared_ptr<CicIntegrator< 25, 24 > > integrator_3_;
   boost::shared_ptr<CicDifferentiator< 24, 23 > > differentiator_4_;
   boost::shared_ptr<CicDifferentiator< 23, 22 > > differentiator_5_;
   boost::shared_ptr<CicDifferentiator< 22, 21 > > differentiator_6_;
   boost::shared_ptr<CicDifferentiator< 21, 20 > > differentiator_7_;

   void ComputeInput(){
      sc_int< 52 > buffer = input.read();
      buffer = buffer << 52-32;
      sig_0_ = buffer;
      
   };

   void ComputeOutput(){
      output = data_output_type( sc_int<20>( sig_8_.read() ).range(19,3) );
   }

   void DivClock(){

      if( idx_++ == (decimation.read() - 1) )
      {
         idx_ = 0;
         div_clock = !div_clock.read();
      }

   }

   typedef bool bit_type;
   typedef sc_int< INPUT_WIDTH > data_input_type;
   typedef sc_int< OUTPUT_WIDTH > data_output_type;

   public:

   SC_HAS_PROCESS( Cic );

   Cic( const sc_module_name& nm) : sc_module(nm){

      SC_METHOD( ComputeInput );
      sensitive << clock.pos();

      SC_METHOD( ComputeOutput );
      sensitive << div_clock.posedge_event();

      SC_METHOD( DivClock );
      sensitive << clock.pos();

      integrator_0_ = boost::shared_ptr<CicIntegrator< 52, 43 > >
         (
          new CicIntegrator< 52,43 >( "integrator_0_" )
         );
      integrator_0_->clock( clock );
      integrator_0_->reset( reset );
      integrator_0_->input( sig_0_ );
      integrator_0_->output( sig_1_ );

      integrator_1_ = boost::shared_ptr<CicIntegrator< 43, 34 > >
         (
          new CicIntegrator< 43,34 >( "integrator_1_" )
         );
      integrator_1_->clock( clock );
      integrator_1_->reset( reset );
      integrator_1_->input( sig_1_ );
      integrator_1_->output( sig_2_ );

      integrator_2_ = boost::shared_ptr<CicIntegrator< 34, 25 > >
         (
          new CicIntegrator< 34,25 >( "integrator_2_" )
         );
      integrator_2_->clock( clock );
      integrator_2_->reset( reset );
      integrator_2_->input( sig_2_ );
      integrator_2_->output( sig_3_ );

      integrator_3_ = boost::shared_ptr<CicIntegrator< 25, 24 > >
         (
          new CicIntegrator< 25,24 >( "integrator_3_" )
         );
      integrator_3_->clock( clock );
      integrator_3_->reset( reset );
      integrator_3_->input( sig_3_ );
      integrator_3_->output( sig_4_ );

      differentiator_4_ = boost::shared_ptr<CicDifferentiator< 24, 23 > >
         (
          new CicDifferentiator< 24,23 >( "differentiator_4_" )
         );
      differentiator_4_->clock( div_clock );
      differentiator_4_->reset( reset );
      differentiator_4_->input( sig_4_ );
      differentiator_4_->output( sig_5_ );

      differentiator_5_ = boost::shared_ptr<CicDifferentiator< 23, 22 > >
         (
          new CicDifferentiator< 23,22 >( "differentiator_5_" )
         );
      differentiator_5_->clock( div_clock );
      differentiator_5_->reset( reset );
      differentiator_5_->input( sig_5_ );
      differentiator_5_->output( sig_6_ );

      differentiator_6_ = boost::shared_ptr<CicDifferentiator< 22, 21 > >
         (
          new CicDifferentiator< 22,21 >( "differentiator_6_" )
         );
      differentiator_6_->clock( div_clock );
      differentiator_6_->reset( reset );
      differentiator_6_->input( sig_6_ );
      differentiator_6_->output( sig_7_ );

      differentiator_7_ = boost::shared_ptr<CicDifferentiator< 21, 20 > >
         (
          new CicDifferentiator< 21,20 >( "differentiator_7_" )
         );
      differentiator_7_->clock( div_clock );
      differentiator_7_->reset( reset );
      differentiator_7_->input( sig_7_ );
      differentiator_7_->output( sig_8_ );

   };

   sc_in_clk clock;
   sc_in< data_input_type > decimation;
   sc_in< bit_type > reset;
   sc_in< data_input_type > input;
   sc_out< data_output_type > output;
};
