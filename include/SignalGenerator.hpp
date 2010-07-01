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
#ifndef SIGNAL_GENERATOR_HPP
#define SIGNAL_GENERATOR_HPP

#include<systemc.h>
#include<vector>

template< unsigned int BIT_WIDTH >
class SignalGenerator : public sc_module
{
   typedef sc_uint<1> bit_type;
   typedef sc_int< BIT_WIDTH > sample_type;

   void Compute()
   {
      static int index = 0;

      if( index == samples_.size() || reset.read() == 1 ) index = 0;

      std::cout << "index = " << index << std::endl;

      output = samples_[index++];
   }

   // provide definition in derived class
   virtual void GenerateSamples() =0;

   protected:

   const int SAMPLE_SIZE;
   std::vector< sample_type > samples_;

   public:

      SC_HAS_PROCESS( SignalGenerator );

      SignalGenerator( const sc_module_name& nm, const int sampleSize):
         sc_module( nm ), SAMPLE_SIZE( sampleSize ), samples_(sampleSize) {
            
            SC_METHOD( Compute );
            sensitive << clock.pos();

         }

      sc_in< bit_type > reset;
      sc_in_clk clock;
      sc_out< sample_type > output;

      // must call this prior to use
      void Init(){
         GenerateSamples();
      }
};

#endif
