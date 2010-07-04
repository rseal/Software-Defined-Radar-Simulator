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
#ifndef GAUSSIAN_NOISE_GENERATOR_HPP
#define GAUSSIAN_NOISE_GENERATOR_HPP

#include<boost/random/mersenne_twister.hpp>
#include<boost/random/normal_distribution.hpp>
#include<boost/random/variate_generator.hpp>

#include<sdr_simulator/util/SignalGenerator.hpp>
#include<tr1/math.h>

// Gaussian noise generator with adjustable mean, variance, and amplitude
template< unsigned int BIT_WIDTH>
class GaussianNoiseGenerator: public SignalGenerator<BIT_WIDTH>
{
   // define constants
   const int SCALE;
   const float MEAN;
   const float VARIANCE;
   const float AMPLITUDE;

   // random number number generating algorithm 
   boost::mt11213b rng;
   // gaussian distribution
   boost::normal_distribution<> nDistribution;

   // compute a new sample on each clock cycle
   virtual void Compute()
   {
      if(!this->reset.read())
         this->output = sc_int< BIT_WIDTH >( getRandomNumber()*SCALE*AMPLITUDE );
   }

   // returns a pseudo-random number based on the mt11213b RNG.
   double getRandomNumber()
   {
      boost::variate_generator< boost::mt11213b&, boost::normal_distribution<> > random_number(rng,nDistribution);
      return random_number();
   }
   
   public:

   SC_HAS_PROCESS( GaussianNoiseGenerator );

   // CTOR
   GaussianNoiseGenerator( 
         const sc_module_name& nm, 
         const float mean = 0.0, 
         const float variance = 1.0, 
         const float amplitude = 1.0
         ):
      SignalGenerator<BIT_WIDTH>( nm , 0), 
      SCALE( std::tr1::pow(2.0,BIT_WIDTH)-1 ), MEAN(mean), VARIANCE(variance),
      AMPLITUDE( amplitude ), nDistribution(MEAN,VARIANCE) { }

};


#endif
