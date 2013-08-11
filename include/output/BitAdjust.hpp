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
#ifndef BIT_ADJUST_HPP
#define BIT_ADJUST_HPP

#include<sdr_simulator/SdrModule.hpp>

// 
template<typename INPUT_TYPE, typename OUTPUT_TYPE>
class BitAdjust: public sdr_module::Module<INPUT_TYPE,OUTPUT_TYPE>
{
	int bitGain_;
	int inputSize_;
	int outputSize_;

	virtual void Compute()
	{
		INPUT_TYPE input = this->input;

		int msb = outputSize_ + bitGain_ - 1;
		int lsb = msb - (outputSize_-1);

		std::cout << "(" << msb << "," << lsb << ")" << std::endl;

		OUTPUT_TYPE out  = input.range(msb,lsb);

		this->output.write( out );
	}

	public:

	SC_HAS_PROCESS( BitAdjust );

	// CTOR
	BitAdjust( const sc_module_name& nm, const int bitGain=0 ): 
		sdr_module::Module<INPUT_TYPE,OUTPUT_TYPE>(nm),
		bitGain_(bitGain)
	{
		inputSize_ = INPUT_TYPE().length();
		outputSize_ = OUTPUT_TYPE().length();
	}

};


#endif
