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
#ifndef GAUSSIAN_NOISE_STIMULUS_HPP
#define GAUSSIAN_NOISE_STIMULUS_HPP

#include<sdr_simulator/GaussianNoiseGenerator.hpp>
#include<sdr_simulator/util/Stimulus.hpp>

#include <systemc.h>
#include <tr1/math.h>
#include <fstream>
#include <boost/math/constants/constants.hpp>
#include <boost/shared_ptr.hpp>

template < typename T, unsigned int BIT_WIDTH >
class GaussianNoiseStimulus: public Stimulus<T>
{
   const double MEAN;
   const double VARIANCE;
   const double AMPLITUDE;

   // create gaussian noise signal source.
   typedef boost::shared_ptr< GaussianNoiseGenerator< BIT_WIDTH > > NoiseGeneratorPtr;
   NoiseGeneratorPtr noiseGenerator;

   // override to connect custom stimulus modules
   virtual void Init()
   {
         // create noise generator
         noiseGenerator = NoiseGeneratorPtr( 
               new GaussianNoiseGenerator< BIT_WIDTH >( 
                  "noiseGen", MEAN, VARIANCE, AMPLITUDE )
               );
         noiseGenerator->reset( this->reset );
         noiseGenerator->clock( this->clock );
         noiseGenerator->output( this->output );
   }

   public:
      SC_HAS_PROCESS( GaussianNoiseStimulus );

      // CTOR
      GaussianNoiseStimulus
         ( 
          const sc_module_name& nm, 
          const double mean,
          const double variance, 
          const double amplitude
         ): Stimulus<T>(nm),MEAN(mean), VARIANCE(variance), 
         AMPLITUDE(amplitude) {
            Init();
         }

};

#endif

