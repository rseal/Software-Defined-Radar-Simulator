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

template<typename RESET_TYPE>
class PulseGenerator : public SignalGenerator< double, RESET_TYPE > 
{
   const double pw_;
   const double pri_;
   const double voltage_;
   const double fSample_;
   const double fSignal_;
   int samplesPerPri_;
   int samplesPerPulse_;
   unsigned int index_;
   double theta_;

   sc_signal< double > null_input_signal_;

   void Compute()
   {

      if( index_ == samplesPerPri_ || this->reset.read() )
      {
         index_ = 0;
         this->output = 0.0;
      }
      else
      {
         this->output = this->samples_[ index_++ ];  
      }

   }

   // Optional method for providing a fixed number of random samples
   void GenerateSamples() {

      double phase = 0.0;
      for( int i=0; i<samplesPerPulse_ ; ++i )
      {
         phase = voltage_*std::tr1::sin( theta_*i ); 

         //std::cout << "theta " << theta_ << std::endl;
         //std::cout << "phase " << phase << std::endl;
         this->samples_[i] = phase;
      };

   }

   public:

   SC_HAS_PROCESS( PulseGenerator );

   // CTOR: Set sampleSize to zero if overriding the Compute method.
   PulseGenerator( 
         const sc_module_name& nm, 
         const double pw, 
         const double pri, 
         const double fSample,
         const double fSignal,
         const double voltage
         ):
      SignalGenerator<double, RESET_TYPE>( nm,0 ), pw_( pw ), pri_( pri ), 
      fSample_( fSample ), fSignal_(fSignal), voltage_( voltage ), index_(0)
   {
      samplesPerPri_   = static_cast<int>( pri_ * fSample_ );
      samplesPerPulse_ = static_cast<int>( pw_ * fSample_ );
      this->samples_.resize( samplesPerPri_ );
      theta_ = 2.0*boost::math::constants::pi<double>()*fSignal/fSample;
      GenerateSamples();
   }

};

#endif
