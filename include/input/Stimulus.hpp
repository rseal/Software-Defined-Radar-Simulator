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

// abstract class to assist in stimulus IO development for hardware testing.
template< typename RESET_TYPE >
class Stimulus: public sc_module
{
   typedef sc_export< sc_signal_inout_if< bool > > clk_out_export;

   const unsigned int RESET_TIME;

   // define internal signals for use with exports
   sc_signal<RESET_TYPE> reset_;
   sc_clock clock_;

   // reset signal
   void Reset() {

      reset_.write( true );
      wait(RESET_TIME, SC_NS);
      reset_.write( false );

   }

   public:
   SC_HAS_PROCESS( Stimulus );

   // CTOR
   Stimulus( const sc_module_name& nm, const sc_time& time,
         const unsigned int resetLength ):
      sc_module(nm), RESET_TIME( resetLength ), clock_( "clock", time )
   {
      SC_THREAD( Reset );
      sensitive << clock_.posedge_event();

      // tie internal clock signal to exported clock output
      clock( clock_ );
      reset( reset_ );
   }

	inline friend void sc_trace(sc_trace_file* tf, const Stimulus& stim, const std::string& name ) 
	{
		sc_trace(tf,stim.clock_, name + ".clock");
		sc_trace(tf,stim.reset, name + ".reset");
	}

	// define port IO
	clk_out_export clock;
	sc_out< RESET_TYPE > reset;
};

#endif

