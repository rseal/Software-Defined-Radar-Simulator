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
#ifndef CLOCK_DIVIDER_HPP
#define CLOCK_DIVIDER_HPP

#include <systemc.h>

class ClockDivider : public sc_module
{
	typedef bool reset_type;
   typedef sc_export< sc_signal_inout_if<bool> > clk_export_out;
	typedef sc_signal<bool> Clock;
   Clock divClock_;

	int idx_;

   virtual void DivideClock()
	{
		if( this->reset)
		{
			idx_=0;
			divClock_=0;
		}
		else if( idx_-- == 0)
		{
			idx_ = DIVISOR-1;
			divClock_ = !divClock_;
		}
	}

	public:

	const int DIVISOR;

	//systemc-specific macro
	SC_HAS_PROCESS( ClockDivider );

	// CTOR: Set sampleSize to zero if overriding the Compute method.
	ClockDivider( const sc_module_name& nm, const int divisor): 
		sc_module(nm), divClock_(0), idx_(divisor), DIVISOR( divisor/2 )
	{ 
		SC_METHOD( DivideClock );
		this->sensitive << this->clock.pos();

		div_clock( divClock_ );
	}

	inline friend void sc_trace(sc_trace_file* tf, const ClockDivider& cd, const std::string& name ) 
	{
		sc_trace(tf,cd.divClock_, name + ".div_clock");
		sc_trace(tf,cd.clock, name + ".clock");
		sc_trace(tf,cd.reset, name + ".reset");
	}


	sc_in_clk clock;
	clk_export_out div_clock;
	sc_in< reset_type > reset;
};

#endif
