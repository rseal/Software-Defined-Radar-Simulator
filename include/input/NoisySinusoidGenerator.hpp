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
#ifndef NOISY_SINUSOID_GENERATOR_HPP
#define NOISY_SINUSOID_GENERATOR_HPP

#include <tr1/math.h>

#include <sdr_simulator/SdrModule.hpp>
#include <sdr_simulator/util/SinusoidGenerator.hpp>
#include <sdr_simulator/util/GaussianNoiseGenerator.hpp>

template<typename DATA_TYPE, typename RESET_TYPE>
class NoisySinusoidGenerator : public sdr_module::Module< DATA_TYPE, DATA_TYPE >
{
   typedef boost::shared_ptr< GaussianNoiseGenerator< DATA_TYPE, RESET_TYPE > > NoiseGenerator;
   typedef boost::shared_ptr< SinusoidGenerator< DATA_TYPE, RESET_TYPE > > SineGenerator;

   NoiseGenerator noise_;
   SineGenerator sinusoid_;

   unsigned int index_;
   sc_signal< DATA_TYPE > null_input_signal_;
   sc_signal< DATA_TYPE > noise_signal_;
   sc_signal< DATA_TYPE > sinusoid_signal_;

   virtual void Compute()
   {
      this->output.write( this->reset.read() ? 0 : sinusoid_signal_.read() + noise_signal_.read() );
   }

   public:

   SC_HAS_PROCESS( NoisySinusoidGenerator );

   // CTOR: Set sampleSize to zero if overriding the Compute method.
   NoisySinusoidGenerator( 
         const sc_module_name& nm, 
         const double normalizedFrequency, 
         const int sampleWidth, 
         const double mean = 0.0, 
         const double variance = 1.0, 
         const double amplitude= 1.0 
         ): sdr_module::Module<DATA_TYPE, DATA_TYPE>( nm )
   {
      //int sampleWidth = std::tr1::pow( 2.0, DATA_TYPE().length()-1.0);

      noise_ = NoiseGenerator( 
            new GaussianNoiseGenerator< DATA_TYPE, RESET_TYPE >( 
               "noise", 
               mean, 
               variance, 
               amplitude) );
      noise_->clock( this->clock );
      noise_->reset( this->reset );
      noise_->output( noise_signal_ );

      sinusoid_ = SineGenerator( 
            new SinusoidGenerator< DATA_TYPE, RESET_TYPE > (
               "sig_gen", 
               normalizedFrequency, 
               sampleWidth, 
               amplitude )
            );
      sinusoid_->clock( this->clock );
      sinusoid_->reset( this->reset );
      sinusoid_->output( sinusoid_signal_ );

      this->input( null_input_signal_ );
   }

};

#endif
