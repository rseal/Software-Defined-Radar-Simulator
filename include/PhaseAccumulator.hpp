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
#ifndef PHASE_ACC_HPP
#define PHASE_ACC_HPP

#include <boost/math/constants/constants.hpp>
#include <boost/shared_ptr.hpp>

template <uint BIT_WIDTH>
class PhaseAccumulator: public sc_module  {

   typedef sc_uint<BIT_WIDTH> data_type;
   typedef sc_uint<1> bit_type;

   data_type stepSize_;

   void Accumulate() { 
      out = reset.read() ? 0U : out.read() + stepSize_; 
   }

   public: 

   SC_HAS_PROCESS(PhaseAccumulator);

   // Constructor 
   PhaseAccumulator( const sc_module_name& name, unsigned int stepSize ):
      sc_module(name), stepSize_(stepSize) {

         SC_METHOD(Accumulate);
         sensitive << clock.pos();
      }

   // port definition
   sc_in< bit_type > reset;
   sc_in_clk clock;
   sc_out<data_type> out;
};

#endif
