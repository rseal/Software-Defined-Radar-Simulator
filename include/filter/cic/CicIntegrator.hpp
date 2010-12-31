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
   sc_bv<INPUT_SIZE> memory_;
   sc_bv<INPUT_SIZE> buffer_;
   sc_int<OUTPUT_SIZE> output_;
   const int SHIFT;

   virtual void Compute(){

      // NOTE: output is a signal and updated after a delta-delay.
      if( !this->reset.read() )
      {
         buffer_ = sc_bv<INPUT_SIZE>( this->input.read() + memory_.to_int() );
         memory_ = buffer_;
         output_ = buffer_.range( INPUT_SIZE-1 , SHIFT ).to_int();

         this->output.write( output_ );
         
         //std::cout << "\ninput width = " << buffer.length() << std::endl;
         //std::cout << "output width = " << output.length() << std::endl;
         //std::cout << "msb = " << buffer.length()-1 << std::endl;
         //std::cout << "lsb = " << SHIFT << std::endl << std::endl;
         //std::cout << "shift = " << SHIFT << std::endl;
         //std::cout << "memory = " << memory_ << std::endl;
         //std::cout << "buffer = " << buffer << std::endl;
         //std::cout << "output = " << output_ << " at " << sc_time_stamp() << endl;
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
