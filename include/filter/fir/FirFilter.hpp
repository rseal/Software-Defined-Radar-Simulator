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
#ifndef FIR_FILTER_HPP
#define FIR_FILTER_HPP

#include <sdr_simulator/SdrModule.hpp>

#include <list>
#include <deque>
#include <stdexcept>

template< 
   typename INPUT_TYPE, 
	typename OUTPUT_TYPE, 
	const int COEFF_WIDTH, 
	const int ACC_WIDTH
	>
	class FirFilter: public sdr_module::Module< INPUT_TYPE, OUTPUT_TYPE>
{
	public:

		SC_HAS_PROCESS( FirFilter );

      const int MAX_OUTPUT_INDEX;
      const int MIN_OUTPUT_INDEX;

		typedef sc_int< COEFF_WIDTH > CoeffType;
		typedef std::vector<CoeffType> CoeffVector;

		// CTOR
      FirFilter( const sc_module_name& nm ):
			sdr_module::Module<INPUT_TYPE, OUTPUT_TYPE>( nm ), initialized_( false ), 
         MAX_OUTPUT_INDEX(ACC_WIDTH-1), MIN_OUTPUT_INDEX(ACC_WIDTH - OUTPUT_TYPE().length()){ 
         }


		void LoadCoefficients( const CoeffVector& coeff ) { 
			coeff_ = coeff;
			queue_.resize( coeff_.size() );
			coeff_iter_ = coeff_.begin();
			initialized_ = true;
		}


	private:


		std::deque< CoeffType > queue_;
		CoeffVector coeff_;
		typename CoeffVector::iterator coeff_iter_;

		sc_int< ACC_WIDTH> sum_;

		bool initialized_;
		int data_output_width_;

		// default implementation
		virtual void Compute()
      {
         if( !this->reset.read() && initialized_ )
         {

            sum_ = 0;

            // delete the oldest sample
            queue_.pop_front();

            // add the latest sample
            queue_.push_back( this->input.read() );

            // compute convolution sum
            coeff_iter_ = coeff_.end();

            for( int i=0; i<queue_.size(); ++i)
            {
               sum_ += queue_[i]* *(--coeff_iter_);
            }

            // trim the output to match the output width
            this->output.write( sum_.range( MAX_OUTPUT_INDEX, MIN_OUTPUT_INDEX ).to_int() );
         }
         else
         {
            if( !initialized_ )
            {
               throw std::runtime_error("No coefficients found! Did you load them?\n");
            }

            sum_ = 0;
            this->output.write( 0 );
         }
      }
};

#endif
