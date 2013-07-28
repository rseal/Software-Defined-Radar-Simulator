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
#ifndef SINUSOID_STIMULUS_HPP
#define SINUSOID_STIMULUS_HPP

#include<sdr_simulator/util/SinusoidGenerator.hpp>
#include<sdr_simulator/util/Stimulus.hpp>

#include <systemc.h>
#include <tr1/math.h>
#include <fstream>
#include <boost/math/constants/constants.hpp>
#include <boost/shared_ptr.hpp>

template < typename T, unsigned int BIT_WIDTH >
class SinusoidStimulus: public Stimulus<T>
{
   const double AMPLITUDE;
   const double NORMALIZED_FREQUENCY;

   // create gaussian noise signal source.
   typedef boost::shared_ptr< SinusoidGenerator< BIT_WIDTH > > SignalGeneratorPtr;
   SignalGeneratorPtr signalGenerator_;

   // override to connect custom stimulus modules
   virtual void Init()
   {
      // create noise generator
      signalGenerator_ = SignalGeneratorPtr( 
            new SinusoidGenerator< BIT_WIDTH >( 
               "sigGen", NORMALIZED_FREQUENCY, AMPLITUDE )
            );
      signalGenerator_->reset( this->reset );
      signalGenerator_->clock( this->clock );
      signalGenerator_->output( this->output );
   }

   public:
   SC_HAS_PROCESS( SinusoidStimulus );

   // CTOR
   SinusoidStimulus 
      ( 
       const sc_module_name& nm, 
       const unsigned int resetTime,
       sc_clock& stimClock,
       const double normalizedFrequency,
       const double amplitude
      ): 
         Stimulus<T>( nm, resetTime, stimClock ), 
         NORMALIZED_FREQUENCY( normalizedFrequency ),
         AMPLITUDE(amplitude) {
            Init();
         }
};

#endif

