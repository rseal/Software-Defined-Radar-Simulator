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
#ifndef STIMULUS_HPP
#define STIMULUS_HPP

#include <systemc.h>
#include <sdr_simulator/Types.hpp>

// abstract class to assist in stimulus IO development for hardware testing.
template< typename T >
class Stimulus: public sc_module
{
   const unsigned int RESET_TIME_;

   // define internal signals for use with exports
   sdr_types::reset_signal reset_;

   // reset signal
   void Reset() {

      reset_.write( true );
      wait(RESET_TIME_, SC_NS);
      reset_.write( false );

   }

   // override to connect custom stimulus modules
   // must call this from derived class
   virtual void Init()
   {
      // TODO: Override and add default signal outputs here
   };

   public:
   SC_HAS_PROCESS( Stimulus );

   // CTOR
   Stimulus( const sc_module_name& nm, const unsigned int resetTime, 
         sc_clock& stimClock ): sc_module(nm), RESET_TIME_( resetTime ) 
   {
      SC_THREAD( Reset );
      sensitive << stimClock.posedge_event();

      // tie internal clock signal to exported clock output
      clock( stimClock );
      reset(reset_);
   }

   // define port IO
   sdr_types::reset_export_out reset;
   sdr_types::clk_export_out clock;
   sc_out< T > output;
};

#endif

