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
#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <systemc>
#include <tr1/math.h>

namespace cordic
{
   const unsigned int DATA_WIDTH= 16;
   const unsigned int PHASE_WIDTH = 16;
   const unsigned int NUM_STAGES = 12;
   typedef sc_int<DATA_WIDTH> INPUT_TYPE;
   typedef sc_int<DATA_WIDTH> OUTPUT_TYPE;
   typedef sc_int<PHASE_WIDTH> PHASE_TYPE;
   
   typedef bool RESET_TYPE;

};

namespace accumulator
{
   const unsigned int DATA_WIDTH = 16;
   const double SAMPLE_RATE = 64e6;
   const double TUNING_FREQUENCY = -1.099435e6;
   typedef sc_int< DATA_WIDTH > DATA_TYPE;

   // static helper function
   static const sc_int< DATA_WIDTH> StepSize() { 
      return rint( std::tr1::pow(2.0,DATA_WIDTH) * TUNING_FREQUENCY / SAMPLE_RATE );
   }
};

  
#endif
