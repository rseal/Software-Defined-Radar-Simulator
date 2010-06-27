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
#ifndef HALF_BAND_FILTER_HPP
#define HALF_BAND_FILTER_HPP

#include <systemc.h>

template < unsigned int BIT_WIDTH>
class HalfBandFilter: public sc_module {

   typedef sc_int<BIT_WIDTH> data_type;
   typedef sc_uint<1> bit_type;

   void Sample()
   {
      //TODO: Implementation
   }


   public:

   SC_HAS_PROCESS( HalfBandFilter );

   //CTOR
   HalfBandFilter(const sc_module_name& nm ): sc_module(nm) {
   
      SC_METHOD( Sample );
      sensitive << clock.pos();

   }

   // define interface
   sc_in_clk clock;
   sc_in< bit_type > reset;
   sc_in<data_type> input;
   sc_out<data_type> output;

};

#endif
