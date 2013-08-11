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
#ifndef IMPULSE_GENERATOR_HPP
#define IMPULSE_GENERATOR_HPP

#include<sdr_simulator/input/SignalGenerator.hpp>
#include<boost/math/constants/constants.hpp>
#include<tr1/math.h>

// Gaussian noise generator with adjustable mean, variance, and amplitude
template<typename DATA_TYPE, typename RESET_TYPE>
class ImpulseGenerator: public SignalGenerator<DATA_TYPE,RESET_TYPE>
{
   // define constants
   const double AMPLITUDE;
   const int START_IDX;

	virtual void GenerateSamples()
	{
		this->samples_[START_IDX] = AMPLITUDE;
	}
	
   public:

   SC_HAS_PROCESS( ImpulseGenerator );

   // CTOR
   ImpulseGenerator( const sc_module_name& nm, const int numSamples, 
         const int startIdx, const double amplitude = 1.0):
      SignalGenerator< DATA_TYPE,RESET_TYPE>( nm , numSamples), 
      START_IDX(startIdx), AMPLITUDE( amplitude ) { this->Init(); }
};


#endif
