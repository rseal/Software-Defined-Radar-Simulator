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
#ifndef CIC_INTEGRATOR_HPP
#define CIC_INTEGRATOR_HPP

#include <systemc.h>

template < unsigned int INPUT_WIDTH, unsigned int OUTPUT_WIDTH > 
class CicIntegrator : public sc_module {

   const unsigned int MSB;
   const unsigned int LSB;

   typedef sc_int< INPUT_WIDTH > data_input_type;
   typedef sc_int< OUTPUT_WIDTH > data_output_type;
   typedef sc_int< INPUT_WIDTH+1 > buffer_type;
   typedef bool bit_type;

   buffer_type memory;

   void Initialize(){

   //TODO: Initialize Modules and interconnects

   }

   void Compute(){

      data_input_type buffer = input.read() + data_input_type(memory);
      data_output_type out = data_output_type( buffer.range(MSB,LSB) );

      // NOTE: output is a signal and updated after a delta-delay.
      if( reset.read() )
      {
         output.write( 0 );
         memory = 0;
      }
      else
      {
         //cout 
         //<< " i = " << input.read() 
         //<< " o = " << out 
         //<< " m = " << memory << endl;
         output.write( out );
         memory = buffer_type( input.read() + memory);
      }
   }

   public:

   SC_HAS_PROCESS( CicIntegrator );

   CicIntegrator( const sc_module_name& nm ): 
      sc_module( nm ),
      MSB( INPUT_WIDTH -1),
      LSB( INPUT_WIDTH - OUTPUT_WIDTH )
   {
      SC_METHOD( Compute );
      sensitive << clock.pos();
   }

   sc_in_clk clock;
   sc_in< bit_type > reset;
   sc_in< data_input_type > input;
   sc_out< data_output_type > output;

};


#endif
