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

#include<systemc.h>

template <typename DATA_TYPE, typename RESET_TYPE >
class PhaseAccumulator: public sc_module  {

   const double STEP_SIZE;
   DATA_TYPE theta;

   void Accumulate() 
   { 
      theta = reset.read() ? 0 : rint( theta + STEP_SIZE ); 
      out.write( theta );
   }

   public: 

   SC_HAS_PROCESS(PhaseAccumulator);

   // Constructor 
   PhaseAccumulator( const sc_module_name& name, const double stepSize ):
      sc_module(name), STEP_SIZE(stepSize)
   {
      SC_METHOD(Accumulate);
      sensitive << clock.pos();
   }

   // port definition
   sc_in<RESET_TYPE> reset;
   sc_in_clk clock;
   sc_out<DATA_TYPE> out;
};

#endif
