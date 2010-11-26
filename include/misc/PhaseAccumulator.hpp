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

#include <sdr_simulator/SdrModule.hpp>

// locally-generated constants file
#include "configuration.hpp"

class PhaseAccumulator:
   public sdr_module::Module<accumulator::DATA_TYPE, accumulator::DATA_TYPE>  {

   const double STEP_SIZE;
   accumulator::DATA_TYPE theta;

   sc_signal< accumulator::DATA_TYPE > null_input_signal_;

   virtual void Compute() 
   { 
      theta = this->reset.read() ? 0 : rint( theta + STEP_SIZE ); 
      this->output.write( theta );
   }

   public: 

   SC_HAS_PROCESS(PhaseAccumulator);

   // Constructor 
   PhaseAccumulator( const sc_module_name& name ):
      sdr_module::Module<accumulator::DATA_TYPE,accumulator::DATA_TYPE>(name), 
      STEP_SIZE( accumulator::StepSize() )
   {
      this->input( null_input_signal_ );
   }
};

#endif
