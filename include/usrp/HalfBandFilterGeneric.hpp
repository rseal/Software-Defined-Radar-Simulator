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
#ifndef HBF_GENERIC_HPP
#define HBF_GENERIC_HPP

#include <systemc.h>
#include <sdr_simulator/FirFilter.hpp>

template < unsigned int INPUT_WIDTH, unsigned int OUTPUT_WIDTH, 
         unsigned int COEFF_WIDTH, unsigned int SUM_WIDTH> 
class HalfBandFilterGeneric: 
   public FirFilter<INPUT_WIDTH,OUTPUT_WIDTH,COEFF_WIDTH,SUM_WIDTH> {

   bool clockToggle_;

   public:

   SC_HAS_PROCESS( HalfBandFilterGeneric );

   //CTOR
   HalfBandFilterGeneric(const sc_module_name& nm ): 
      FirFilter<INPUT_WIDTH,OUTPUT_WIDTH,COEFF_WIDTH,SUM_WIDTH>(nm),
      clockToggle_(false)
   {
      // poor implementation of a 31-tap half-band filter
         this->coeff_.push_back( -49 );
         this->coeff_.push_back( 0 );
         this->coeff_.push_back( 165 );
         this->coeff_.push_back( 0 );
         this->coeff_.push_back( -412 );
         this->coeff_.push_back( 0 );
         this->coeff_.push_back( 873 );
         this->coeff_.push_back( 0 );
         this->coeff_.push_back( -1681 );
         this->coeff_.push_back( 0 );
         this->coeff_.push_back( 3135 );
         this->coeff_.push_back( 0 );
         this->coeff_.push_back( -6282 );
         this->coeff_.push_back( 0 );
         this->coeff_.push_back( 20628 );
         this->coeff_.push_back( 32767 );
         this->coeff_.push_back( 20628 );
         this->coeff_.push_back( 0 );
         this->coeff_.push_back( -6282 );
         this->coeff_.push_back( 0 );
         this->coeff_.push_back( 3135 );
         this->coeff_.push_back( 0 );
         this->coeff_.push_back( -1681 );
         this->coeff_.push_back( 0 );
         this->coeff_.push_back( 873 );
         this->coeff_.push_back( 0 );
         this->coeff_.push_back( -412 );
         this->coeff_.push_back( 0 );
         this->coeff_.push_back( 165 );
         this->coeff_.push_back( 0 );
         this->coeff_.push_back( -49 );
         this->queue_.resize( this->coeff_.size() );
   }

};

#endif
