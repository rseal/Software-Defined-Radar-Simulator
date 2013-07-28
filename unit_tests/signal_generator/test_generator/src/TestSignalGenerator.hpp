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
#ifndef TEST_SIGNAL_GENERATOR_HPP
#define TEST_SIGNAL_GENERATOR_HPP

#include<boost/random/linear_congruential.hpp>
#include<boost/random/normal_distribution.hpp>
#include<boost/random/variate_generator.hpp>

#include<sdr_simulator/input/SignalGenerator.hpp>
#include<iostream>
#include<tr1/math.h>

#include "test_bench.hpp"

template<const int BIT_WIDTH, typename OUTPUT_TYPE, typename RESET_TYPE>
class TestSignalGenerator: public SignalGenerator<OUTPUT_TYPE,RESET_TYPE>
{
   const int SCALE;
   const double AMPLITUDE;

   // random number number generating algorithm 
   boost::rand48 rng;

   virtual void GenerateSamples() {
      for( int i=0; i< this->SAMPLE_SIZE; ++i) {
         this->samples_[i] = sc_int<BIT_WIDTH>(getRandomNumber()*SCALE*AMPLITUDE);
      }
   }

   double getRandomNumber()
   {
      // gaussian distribution
      boost::normal_distribution<double> nDistribution(0.0,1.0);
      // create the random number generator
      boost::variate_generator< boost::rand48&, boost::normal_distribution<double> > random_number(rng,nDistribution);

      return random_number();
   }


   public:

   SC_HAS_PROCESS( TestSignalGenerator );

   TestSignalGenerator( const sc_module_name& nm, const int sampleSize, const double amplitude):
      SignalGenerator<OUTPUT_TYPE,RESET_TYPE>( nm, sampleSize ), SCALE( std::tr1::pow(2.0,BIT_WIDTH)-1 ),
      AMPLITUDE( amplitude )
   {
      this->Init();
   }

};


#endif
