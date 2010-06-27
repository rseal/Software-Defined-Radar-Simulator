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

template < uint INPUT_WIDTH, uint OUTPUT_WIDTH>
class CordicShiftAdder : public sc_module
{
   typedef boost::shared_ptr<ofstream> OutputFileStream;
   OutputFileStream ofStream_;

   typedef sc_int<INPUT_WIDTH> cordic_input_type; 
   typedef sc_int<OUTPUT_WIDTH> cordic_output_type; 
   typedef sc_uint<1> cordic_bit_type;

   const int SHIFT_;

   void compute()
   {
      int s = sign.read() == 0 ? 1 : -1;

      if(!reset.read()){
         out = in_a.read() + s*(in_b.read()>>SHIFT_);
         //Display();
      }
      else{
         out = 0;
      }

   }

   void Display() {
         *ofStream_
            << std::setw(10) << this->name()
            << " a = " << std::setw(6) << in_a.read() 
            << " b = " << std::setw(6) << in_b.read() 
            << " s = " << std::setw(3) << (sign.read() == 0 ? 1 : -1)
            << " o = " << std::setw(6) << out.read() 
            << " at  " << sc_time_stamp()
            << endl;
   }

   public:

   SC_HAS_PROCESS(CordicShiftAdder);

   CordicShiftAdder( sc_module_name nm, const int& shift ): sc_module(nm), SHIFT_(shift)
   {
      SC_METHOD(compute);
      sensitive << in_a << in_b << sign << reset;

      //DEBUG ONLY
      //string name = this->name();
      //name += ".dat";
      //ofStream_ = OutputFileStream( new ofstream( name.c_str(), ios::out ) );
   }

   sc_in_clk clock;
   sc_in<cordic_bit_type> reset;
   sc_in<cordic_input_type> in_a;
   sc_in<cordic_input_type> in_b;
   sc_out<cordic_output_type> out;
   sc_in<cordic_bit_type> sign;
};

#endif
