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
#ifndef ADC_HPP
#define ADC_HPP

#include <boost/math/constants/constants.hpp>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <tr1/math.h>
#include <ctime>
#include <systemc.h>
#include <sdr_simulator/SdrModule.hpp>

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
template< typename OUTPUT_TYPE, int BIT_WIDTH >
class Adc : public sdr_module::Module< double, OUTPUT_TYPE >
{

	// random number generator
   typedef boost::mt19937 Engine;
   typedef boost::normal_distribution<> Distribution;
   typedef boost::variate_generator< Engine&, Distribution > Generator;

	// random number number generating algorithm
	Engine rng_;
	Generator randomNumber_;

	double vfs_;
	double snr_;
	double bw_;
	double noise_;
	double fs_;
	double stepSize_;

	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	virtual void Compute()
	{
		if ( !this->reset.read() )
		{
			// add gaussian noise based on SNR
			double buffer = (this->input.read() + noise_*randomNumber_())/stepSize_;

			// clip if above limit
			if(buffer > vfs_) buffer=vfs_;

			this->output = OUTPUT_TYPE ( buffer );
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	void ComputeNoise()
	{
		double process_gain = fs_/(2.0*bw_);
		double vfs_rms = vfs_/tr1::sqrt(2.0);
		noise_ = vfs_rms * tr1::pow(10.0,-snr_/20.0)/process_gain;
	}

	public:

	SC_HAS_PROCESS( Adc );

	/////////////////////////////////////////////////////////////////////////////
	//CTOR
	/////////////////////////////////////////////////////////////////////////////
	Adc(
			const sc_module_name& nm,
			const double vfs, 
			const double fs,
			const double snr, 
			const double bw
		): 
      sdr_module::Module<double, OUTPUT_TYPE>( nm ), vfs_(vfs), fs_(fs), 
		snr_(snr), bw_(bw), rng_(std::time(0)), 
		randomNumber_( rng_, Distribution(0.0,1.0) ),
		stepSize_(vfs/(std::tr1::pow(2.0,1.0*BIT_WIDTH)-1.0))
	{
		// compute noise value based on input parameters.
		this->ComputeNoise();
	}

	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
   void Seed ( const int seed )
   {
      rng_.seed ( seed );
   }

};

#endif
