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

#include <sdr_simulator/SdrModule.hpp>

#include <systemc.h>

template < int INPUT_SIZE, int OUTPUT_SIZE > 
class CicIntegrator : public sdr_module::Module< sc_int<INPUT_SIZE>, sc_int<OUTPUT_SIZE> >
{
   const int SHIFT;
	sc_int<INPUT_SIZE> memory_;

   virtual void Compute(){

      if( !this->reset.read() )
      {
         // untrimmed output y[n] = x[n] + y[n-1]
         sc_int<INPUT_SIZE> buffer = this->input.read() + memory_;


         // trim LSB bits from input side and assign to output
         sc_int<OUTPUT_SIZE> out = buffer.range(INPUT_SIZE-1,SHIFT);

         // write output y[n]
         this->output.write( out );

			// store y[n]
			memory_ = buffer;
      }
      else
      {
         this->output.write( 0 );
         memory_ = 0;
      }
   }

   public:

   SC_HAS_PROCESS( CicIntegrator );

   CicIntegrator( const sc_module_name& nm ): 
      sdr_module::Module< sc_int<INPUT_SIZE>, sc_int<OUTPUT_SIZE> >( nm ),
      SHIFT( INPUT_SIZE - OUTPUT_SIZE )
   { }
};


#endif
