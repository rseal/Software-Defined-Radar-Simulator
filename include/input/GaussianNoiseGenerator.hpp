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

#include<sdr_simulator/input/SignalGenerator.hpp>
#include<tr1/math.h>
#include<ctime>

// Gaussian noise generator with adjustable mean, variance, and amplitude
template< typename DATA_TYPE, typename RESET_TYPE >
class GaussianNoiseGenerator: public SignalGenerator< DATA_TYPE, RESET_TYPE>
{
   // define constants
   const float MEAN;
   const float VARIANCE;

   typedef boost::mt19937 Engine;
   typedef boost::normal_distribution<> Distribution;
   typedef boost::variate_generator< Engine&, Distribution > Generator;

   // random number number generating algorithm
   Engine rng;
   Generator randomNumber_;

   // compute a new sample on each clock cycle
   virtual void Compute()
   {
      this->output = this->reset.read() ? DATA_TYPE(0) : DATA_TYPE ( randomNumber_());
   }

public:

   SC_HAS_PROCESS ( GaussianNoiseGenerator );

   // CTOR
   GaussianNoiseGenerator (
      const sc_module_name& nm,
      const double mean = 0.0,
      const double variance = 1.0,
      const double amplitude = 1.0
   ) : SignalGenerator<DATA_TYPE,RESET_TYPE> ( nm , 0 ),
   MEAN ( mean ), VARIANCE ( variance ), rng(std::time(0)), 
	randomNumber_( rng, Distribution(mean,variance))
   { }

   void Seed ( const int seed )
   {
      rng.seed ( seed );
   }

};


#endif
