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

#include<systemc.h>

#include<sdr_simulator/Settings.hpp>
#include<sdr_simulator/Types.hpp>
#include<sdr_simulator/PhaseAccumulator.hpp>
#include<sdr_simulator/Cordic.hpp>
#include<sdr_simulator/Cic.hpp>
#include<sdr_simulator/usrp/HalfBandFilterGeneric.hpp>

using namespace settings;

template < unsigned int INPUT_WIDTH, unsigned int OUTPUT_WIDTH >
class ReceiveChain: public sc_module { 

   // cordic definitions
   typedef Cordic<CORDIC::XY_WIDTH, CORDIC::Z_WIDTH> CordicModule;
   typedef boost::shared_ptr< CordicModule > CordicPtr;
   CordicPtr cordic_;

   // phase accumulator definitions
   typedef PhaseAccumulator<ACCUMULATOR::BIT_WIDTH> AccumulatorModule;
   typedef boost::shared_ptr< AccumulatorModule > AccumulatorPtr;
   AccumulatorPtr accumulator_;

   // cic definitions
   typedef Cic< CIC::INPUT_WIDTH, CIC::OUTPUT_WIDTH > CicModule;
   typedef boost::shared_ptr< CicModule > CicPtr;
   CicPtr cic_;

   // half band filter definitions
   typedef HalfBandFilterGeneric< 
      HALF_BAND_FILTER::INPUT_WIDTH,
      HALF_BAND_FILTER::OUTPUT_WIDTH,
      HALF_BAND_FILTER::SUM_WIDTH,
      HALF_BAND_FILTER::COEFF_WIDTH > HalfBandFilterModule;
   typedef boost::shared_ptr< HalfBandFilterModule > HalfBandFilterPtr;
   HalfBandFilterPtr halfBandFilter_;
   
   // data type definitions
   typedef bool bit_type;
   typedef sc_uint< ACCUMULATOR::BIT_WIDTH > phase_out_type;
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
   sc_signal < data_input_type > port_input_signal;
   sc_signal < data_input_type > y_data_in_signal;
   sc_signal < data_output_type > x_data_out_signal;
   sc_signal < data_output_type > y_data_out_signal;
   sc_signal < cic_output_type > cic_output_signal; 
   sc_signal < cic_decimation_type > cic_decimation_signal; 
   sc_signal < hbf_output_type > port_output_signal; 

   // wrapper to initialize all modules
   void Initialize()
   {
      InitializeSignals();
      InitializeNCO();
      InitializeAccumulator();
      InitializeFilter();

   }

   // setup signal constants
   void InitializeSignals()
   {
      // set cordic y-input to gnd.
      y_data_in_signal.write(0);
      cic_decimation_signal.write( CIC::DECIMATION );
   }

   // setup down-converter module
   void InitializeNCO()
   {
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
   void InitializeAccumulator()
   {
      // initialize accumulator signals
      accumulator_ = AccumulatorPtr( 
            new AccumulatorModule( ACCUMULATOR::NAME, ACCUMULATOR::StepSize() ) 
            );
      accumulator_->reset ( reset );
      accumulator_->clock ( clock );
      accumulator_->out ( phase_out_signal );

   }

   // setup filter stages
   void InitializeFilter()
   {
      cic_ = CicPtr( new CicModule( CIC::NAME ) );
      cic_->clock( clock );
      cic_->decimation( cic_decimation_signal );
      cic_->input( x_data_out_signal );
      cic_->output( cic_output_signal );
      cic_->reset( reset );

      halfBandFilter_ = HalfBandFilterPtr( 
            new HalfBandFilterModule( HALF_BAND_FILTER::NAME )
            );
      halfBandFilter_->clock( clock );
      halfBandFilter_->reset(reset );
      halfBandFilter_->input( cic_output_signal );
      halfBandFilter_->output( output );
   }

   // computation performed on each clock cycle
   void Compute()
   {
      if(!reset.read())
      {
         // write the upper 16 bits of the phase accumulator to the cordic z input
         phase_in_signal.write(
               sc_int< ACCUMULATOR::BIT_WIDTH >( phase_out_signal.read() ).range( 
                  ACCUMULATOR::BIT_WIDTH-1, 
                  ACCUMULATOR::BIT_WIDTH - CORDIC::Z_WIDTH ) 
               );
      }
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
   sc_out < data_output_type > output;

};

#endif
