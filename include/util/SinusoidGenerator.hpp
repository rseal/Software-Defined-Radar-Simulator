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
#ifndef SINUSOID_GENERATOR_HPP
#define SINUSOID_GENERATOR_HPP

#include<sdr_simulator/util/SignalGenerator.hpp>
#include<tr1/math.h>

// Gaussian noise generator with adjustable mean, variance, and amplitude
template< unsigned int BIT_WIDTH>
class SinusoidGenerator: public SignalGenerator<BIT_WIDTH>
{
   // define constants
   const double AMPLITUDE;
   const double NORMALIZED_FREQUENCY;
   const int SCALE;
   const double TWO_PI;
   int time_;

   // compute a new sample on each clock cycle
   virtual void Compute()
   {
      if(!this->reset.read()) {
         this->output = sc_int< BIT_WIDTH >( SCALE*AMPLITUDE*tr1::sin(time_++*TWO_PI*NORMALIZED_FREQUENCY));
      }
   }

   public:

   SC_HAS_PROCESS( SinusoidGenerator );

   // CTOR
   SinusoidGenerator( 
         const sc_module_name& nm, 
         const double normalizedFrequency,
         const double amplitude = 1.0
         ):
      SignalGenerator<BIT_WIDTH>( nm , 0), 
      NORMALIZED_FREQUENCY( normalizedFrequency ),  
      AMPLITUDE( amplitude/2.0 ),
      SCALE( std::tr1::pow(2.0,BIT_WIDTH)-1 ), 
      TWO_PI(2.0f*boost::math::constants::pi<float>()),
      time_(0)
   {}
};


#endif
