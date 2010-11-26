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
#ifndef CORDIC_SHIFT_ADDER
#define CORDIC_SHIFT_ADDER


#include <systemc.h>

#include <tr1/math.h>
#include <iomanip>
#include <fstream>
#include <boost/shared_ptr.hpp>


using namespace std;

template< typename INPUT_DATA_TYPE, typename OUTPUT_DATA_TYPE, typename RESET_TYPE >
class CordicShiftAdder : public sc_module
{
   typedef boost::shared_ptr<ofstream> OutputFileStream;
   OutputFileStream ofStream_;

   typedef sc_uint<1> cordic_bit_type;

   const int SHIFT_;

   void compute()
   {
      int s = sign.read() == 0 ? 1 : -1;

      if(!reset.read()){
         output.write( input_a.read() + s*(input_b.read()>>SHIFT_) );
      }
      else{
         output.write( 0 );
      }

   }

   public:

   SC_HAS_PROCESS(CordicShiftAdder);

   CordicShiftAdder( sc_module_name nm, const int& shift ): sc_module(nm), SHIFT_(shift)
   {
      SC_METHOD(compute);
      sensitive << input_a << input_b << sign << reset;

   }

   sc_in_clk clock;
   sc_in< RESET_TYPE > reset;
   sc_in< INPUT_DATA_TYPE > input_a;
   sc_in< INPUT_DATA_TYPE > input_b;
   sc_out< OUTPUT_DATA_TYPE > output;
   sc_in<cordic_bit_type> sign;
};

#endif
