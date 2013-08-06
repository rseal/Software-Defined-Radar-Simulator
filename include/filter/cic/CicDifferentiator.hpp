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
#ifndef CIC_DIFFERENTIATOR_HPP
#define CIC_DIFFERENTIATOR_HPP

#include <iostream>

#include <sdr_simulator/SdrModule.hpp>
#include <systemc.h>

template< int INPUT_SIZE, int OUTPUT_SIZE > class CicDifferentiator: 
	public sdr_module::Module< sc_int<INPUT_SIZE>, sc_int<OUTPUT_SIZE> > 
{
	const int SHIFT;
	sc_int<INPUT_SIZE> memory_;

	virtual void Compute()
	{
		if( !this->reset.read() )
		{
			// Read the input signal.
			sc_int<INPUT_SIZE> buffer = this->input.read();

			// Subtract the memory register from the buffer.
			sc_int<INPUT_SIZE> out = sc_int<INPUT_SIZE>( buffer - memory_ );

			// Update the memory register with the input signal.
			memory_ = buffer;

			// Write to the output signal. Convert to double to avoid bit-width issues.
			this->output.write( out.range(INPUT_SIZE-1,SHIFT).to_double() );
		}
		else
		{
			this->output = 0;
			memory_ = 0;
		}
	}

	public:

	SC_HAS_PROCESS( CicDifferentiator );

	CicDifferentiator( const sc_module_name& nm ): 
		sdr_module::Module< sc_int<INPUT_SIZE>, sc_int<OUTPUT_SIZE> > ( nm ),
		SHIFT( INPUT_SIZE - OUTPUT_SIZE ), memory_(0) 
	{
	}
};

#endif
