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

#include <exception>
#include <sdr_simulator/SdrModule.hpp>

template<typename INPUT_TYPE, typename OUTPUT_TYPE>
class PhaseAccumulator: public sdr_module::Module<INPUT_TYPE,OUTPUT_TYPE>{

   INPUT_TYPE theta;
   double stepSize_;
   const int LSB;
   const int MSB;

   sc_signal< INPUT_TYPE > null_input_signal_;

   virtual void Compute() 
   { 
      theta = this->reset.read() ? 0 : rint( theta + stepSize_ ); 
      this->output.write(theta.range( MSB, LSB));
   }

   public: 

   SC_HAS_PROCESS(PhaseAccumulator);

   // Constructor 
   PhaseAccumulator(const sc_module_name& name): 
      sdr_module::Module<INPUT_TYPE,OUTPUT_TYPE>(name), 
      stepSize_(-1.0), MSB( INPUT_TYPE().length()-1), 
      LSB(INPUT_TYPE().length() - OUTPUT_TYPE().length()) 
   {
      if( LSB < 0)
      {
         throw new std::runtime_error("ERROR: PhaseAccumulator - LSB < 0");
      }

      this->input( null_input_signal_ );
   }

   void StepSize( const double stepSize)
   {
      stepSize_ = stepSize;
   }

};

#endif
