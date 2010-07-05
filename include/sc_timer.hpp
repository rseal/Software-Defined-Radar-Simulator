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
#ifndef SC_TIMER_HPP
#define SC_TIMER_HPP

#include <systemc.h>

class sc_timer: public sc_module  {

	typedef bool reset_type;
   typedef sc_uint<1> bit_type;

   unsigned int timeOutValue_;
   unsigned int value_;

   // main function
   void Execute() { 
      time_out =  value_ == 0U ? 1 : 0;
      value_ =  reset.read() ? timeOutValue_ : value_ - 1U;
   }

   public: 

   SC_HAS_PROCESS(sc_timer);

   // port definition
   sc_in< reset_type > reset;
   sc_in_clk clk;
   sc_out<bit_type> time_out;

   // Constructor 
   sc_timer( const sc_module_name& name, unsigned int timeOutValue):
      sc_module(name), timeOutValue_(timeOutValue), value_(timeOutValue) {
         SC_METHOD(Execute);
         sensitive << clk.pos();
      }
};

#endif
