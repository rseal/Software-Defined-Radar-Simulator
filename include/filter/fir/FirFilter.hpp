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

template < typename INPUT_DATA_TYPE, typename OUTPUT_DATA_TYPE, 
         typename COEFF_TYPE, unsigned int SUM_WIDTH >
class FirFilter: public sdr_module::Module< INPUT_DATA_TYPE, OUTPUT_DATA_TYPE>
{
  bool initialized_;

  std::deque< typename COEFF_TYPE::value_type > queue_;
  COEFF_TYPE coeff_;
  typename COEFF_TYPE::iterator coeff_iter_;
  sc_int< SUM_WIDTH> sum_;

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
        this->output.write( 
              sum_.range( SUM_WIDTH-1, SUM_WIDTH-data_output_width_ ) 
              );
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


public:

  SC_HAS_PROCESS( FirFilter );

  // CTOR
  FirFilter( const sc_module_name& nm ):
     sdr_module::Module<INPUT_DATA_TYPE,OUTPUT_DATA_TYPE>( nm ), 
     initialized_( false ) { }

  void LoadCoefficients( COEFF_TYPE& coeff ) { 
      coeff_ = coeff;
      queue_.resize( coeff.size() );
      coeff_iter_ = coeff.begin();
      data_output_width_ = OUTPUT_DATA_TYPE().length();
      initialized_ = true;
   }
};

#endif
