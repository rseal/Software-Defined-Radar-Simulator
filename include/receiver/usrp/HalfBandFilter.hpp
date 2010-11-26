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
#ifndef USRP_HBF_HPP
#define USRP_HBF_HPP

#include <systemc.h>
#include <vector>
#include <sdr_simulator/FirFilter.hpp>

class UsrpHalfBandFilter: public FirFilter<16,16,16,34> {

   typedef sc_int< 16 > coeff_type;
   typedef sc_uint<1> bit_type;

   std::vector< coeff_type > evenStorageBank_;
   std::vector< coeff_type > oddStorageBank_;
   boolean clockToggle_;

   sc_signal< bit_type > strobe_in_signal_;
   sc_signal< bit_type > strobe_out_signal_;

   virtual void Compute()
   {
      clockToggle_ != clockToggle_;
   }

   public:

   //CTOR
   UsrpHalfBandFilter(const sc_module_name& nm ): 
      FirFilter<16,16,16,34>(nm),
      evenStorageBank_(16,0), oddStorageBank_(16,0),
      clockToggle_(false)
   {
         coeff_.push_back( -49 );
         coeff_.push_back( 165 );
         coeff_.push_back( -412 );
         coeff_.push_back( 873 );
         coeff_.push_back( -1681 );
         coeff_.push_back( 3135 );
         coeff_.push_back( 6282 );
         coeff_.push_back( 20628 );
   }

};

#endif
