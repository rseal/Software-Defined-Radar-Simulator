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
#ifndef PULSE_GENERATOR_HPP
#define PULSE_GENERATOR_HPP

#include <sdr_simulator/util/SignalGenerator.hpp>
#include<tr1/math.h>
#include<boost/math/constants/constants.hpp>
#include<vector>

namespace pulse_generator{
   const static double TWO_PI = 2.0*boost::math::constants::pi<double>();
};

template<typename DATA_TYPE, typename RESET_TYPE>
class PulseGenerator : public SignalGenerator< DATA_TYPE, RESET_TYPE > 
{
   const double PULSE_WIDTH;
   const double IPP_WIDTH;
   const double AMPLITUDE;
   unsigned int index_;
   double theta_;

   sc_signal< DATA_TYPE > null_input_signal_;

   void Compute()
   {

     if( index_ == IPP_WIDTH || this->reset.read() )
     {
        index_ = 0;
        this->output = 0;
     }
     else
     {
        this->output = this->samples_[ index_++ ];  
     }

   }

   // Optional method for providing a fixed number of random samples
   void GenerateSamples() {

      double phase = 0.0;
     for( int i=0; i<PULSE_WIDTH; ++i )
     {
        phase = AMPLITUDE*std::tr1::sin( theta_*i ); 
        this->samples_[i] = phase;
     };

   }

   public:

   SC_HAS_PROCESS( PulseGenerator );

   // CTOR: Set sampleSize to zero if overriding the Compute method.
   PulseGenerator( 
         const sc_module_name& nm, 
         const int pulseWidth, 
         const double ippWidth, 
         const double normalizedFrequency,
         const double amplitude
         ):
      SignalGenerator<DATA_TYPE, RESET_TYPE>( nm,0 ), 
      PULSE_WIDTH( pulseWidth ), IPP_WIDTH( ippWidth ), 
      AMPLITUDE( amplitude ), index_(0)
   {
      this->samples_.resize( IPP_WIDTH );

      theta_ = 2.0*boost::math::constants::pi<double>()*normalizedFrequency;
      GenerateSamples();
   }

};

#endif
