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
#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <tr1/math.h>

namespace settings{

   namespace HALF_BAND_FILTER{
      const char* NAME = "half_band_filter";
      const unsigned int INPUT_WIDTH = 16;
      const unsigned int OUTPUT_WIDTH = 16;
      const unsigned int SUM_WIDTH = 37;
      const unsigned int COEFF_WIDTH = 16; 
   };

   namespace ADC{
      const char* NAME="adc";
      const unsigned int BIT_WIDTH = 12;
   };

   namespace CIC{
      const char* NAME="cic";
      const unsigned int NUM_STAGES = 4;
      const unsigned int INPUT_WIDTH = 16;
      const unsigned int OUTPUT_WIDTH = 16;
      const unsigned int DECIMATION = 4;
      const unsigned int DECIMATION_WIDTH = 16;
   };

   namespace CORDIC{
      const char* NAME="cordic";
      const unsigned int NUM_STAGES = 12;
      const unsigned int XY_WIDTH = 16;
      const unsigned int Z_WIDTH = 16;
   };

   namespace ACCUMULATOR{
      const char* NAME="accumulator";
      const float SAMPLE_RATE = 64e6;
      const float SIGNAL_RATE = 14e6;
      const unsigned int BIT_WIDTH = 48;

      // static helper function
      static const sc_int< BIT_WIDTH> StepSize() { 
         return sc_int< BIT_WIDTH > 
            ( std::tr1::pow(2.0,BIT_WIDTH*1.0) * SIGNAL_RATE / SAMPLE_RATE );
      };
   };


};
#endif
