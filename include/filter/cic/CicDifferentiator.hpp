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
#ifndef CIC_DIFFERENTIATOR_HPP
#define CIC_DIFFERENTIATOR_HPP

#include <sdr_simulator/SdrModule.hpp>

#include <systemc.h>

template< int INPUT_SIZE, int OUTPUT_SIZE > 
class CicDifferentiator: 
   public sdr_module::Module< sc_int<INPUT_SIZE>, sc_int<OUTPUT_SIZE> > 
{
   const int SHIFT;
   sc_bv<INPUT_SIZE> memory_;
   sc_bv<INPUT_SIZE> buffer_;

   virtual void Compute()
   {
      if( !this->reset.read() )
      {
         //cout << "\n\n" << "TIME: " << sc_time_stamp() << "\n";
         //cout << this->name() << ".input  = " << this->input.read() << "\n";
         //cout << this->name() << ".memory = " << memory_ << "\n";

         buffer_ =  sc_bv<INPUT_SIZE>( this->input.read() - memory_.to_int() );

         sc_bv<OUTPUT_SIZE> out = sc_bv<OUTPUT_SIZE>( buffer_.range( INPUT_SIZE-1, SHIFT ) );

         memory_ = sc_bv<INPUT_SIZE>( this->input.read() );
         this->output.write( out.to_int() );

         //cout << "\n" << this->name() << ".buffer = " << buffer_ << "\n";
         //cout << this->name() << ".output = " << out << "\n";
         //cout << "\n\n";

      }
      else
      {
         this->output.write( 0 );
         memory_ = 0;
      }
   }

   public:

   SC_HAS_PROCESS( CicDifferentiator );

   CicDifferentiator( const sc_module_name& nm ): 
      sdr_module::Module< sc_int<INPUT_SIZE>, sc_int<OUTPUT_SIZE> > ( nm ),
      SHIFT( INPUT_SIZE - OUTPUT_SIZE ), memory_(0) { }
};

#endif
