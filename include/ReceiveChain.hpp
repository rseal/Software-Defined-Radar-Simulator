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
#include<sdr_simulator/PhaseAccumulator.hpp>
#include<sdr_simulator/Cordic.hpp>
#include<sdr_simulator/Cic.hpp>
#include<sdr_simulator/HalfBandFilter.hpp>

using namespace settings;

template < unsigned int INPUT_WIDTH, unsigned int OUTPUT_WIDTH >
class ReceiveChain: sc_module { 

   typedef Cordic<CORDIC::XY_WIDTH, CORDIC::Z_WIDTH> CordicModule;
   typedef PhaseAccumulator<PHASE_ACCUMULATOR::BIT_WIDTH> AccumulatorModule;
   typedef Cic< CIC::INPUT_WIDTH, CIC::OUTPUT_WIDTH > CicModule;
   typedef HalfBandFilter< HALF_BAND_FILTER::BIT_WIDTH > HalfBandFilterModule;
   
   typedef sc_uint<1> bit_type;
   typedef sc_uint< PHASE_ACCUMULATOR::BIT_WIDTH > phase_type;
   typedef sc_int< INPUT_WIDTH > data_input_type;
   typedef sc_int< OUTPUT_WIDTH > data_output_type;

   CordicModule cordic_;
   AccumulatorModule accumulator_;
   CicModule cic_;
   HalfBandFilterModule halfBandFilter_;

   //sc_in_clk clock_signal;
   sc_signal < phase_type > phase_signal;
   //sc_signal < data_type > data_in_signal;
   //sc_signal < data_type > data_out_signal;

   void Initialize()
   {
      

   }

   public:

   //CTOR
   ReceiveChain(): 
      cordic_(CORDIC::NAME, CORDIC::NUM_STAGES),
      accumulator_ ( 
            PHASE_ACCUMULATOR::NAME, 
            PHASE_ACCUMULATOR::StepSize(), 
            PHASE_ACCUMULATOR::USE_LOGGING, 
            PHASE_ACCUMULATOR::USE_DISPLAY
            ),
      cic_( CIC::NAME ),
      halfBandFilter_( HALF_BAND_FILTER::NAME )
   {
      Initialize();
      
      // initialize accumulator signals
      accumulator_.reset ( reset );
      accumulator_.clock ( clock );
      accumulator_.out ( phase_signal );

      //// initialize cordic signals
      //cordic_.clock ( clock );
      //cordic_.reset ( reset );
      //cordic_.xin ( data_in_signal );
      //cordic_.yin(0);
      //cordic_.zin( phase_signal );
      //cordic_.out( data_out_signal );

   }

   // define interface
   sc_in_clk clock;
   sc_in < bit_type > reset;
   sc_in < data_input_type > input;
   sc_out < data_output_type > output;

};

#endif
