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
#include <sdr_simulator/filter/fir/FirFilter.hpp>

template < typename INPUT_DATA_TYPE, typename OUTPUT_DATA_TYPE,
         typename COEFF_TYPE, unsigned int SUM_WIDTH> 
class HalfBandFilterGeneric: 
   public FirFilter<INPUT_DATA_TYPE,OUTPUT_DATA_TYPE,COEFF_TYPE,SUM_WIDTH> {

   typedef sc_export< sc_signal_inout_if<bool> > sc_export_clk;
   sc_signal< bool > div_clock_;

   void DivClock( )
   {
      div_clock_ = !div_clock_.read();
   }

   public:

   SC_HAS_PROCESS( HalfBandFilterGeneric );

   //CTOR
   HalfBandFilterGeneric(const sc_module_name& nm ): 
      FirFilter<INPUT_DATA_TYPE,OUTPUT_DATA_TYPE,COEFF_TYPE,SUM_WIDTH>(nm )
   {
      SC_METHOD( DivClock );
      this->sensitive << this->clock.pos();

      div_clock( div_clock_ );

   }

   sc_export_clk div_clock;
};

#endif
