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

#include "configuration.hpp"

namespace
{
   const int MAX_OUTPUT_INDEX = filter::SUM_WIDTH-1;
   const int MIN_OUTPUT_INDEX = filter::SUM_WIDTH - filter::DATA_WIDTH;
}

class FirFilter: 
   public sdr_module::Module< filter::INPUT_TYPE, filter::OUTPUT_TYPE>
{
  bool initialized_;

  std::deque< filter::coeff_list::value_type > queue_;
  filter::coeff_list coeff_;
  filter::coeff_list::iterator coeff_iter_;
  sc_int< filter::SUM_WIDTH> sum_;

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
             std::cout << "queue sample = " << queue_[i] << std::endl;
             std::cout << "coeff = " << *coeff_iter_ << std::endl;
             std::cout << "sum = " << sum_ << std::endl;
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


public:

  SC_HAS_PROCESS( FirFilter );

  // CTOR
  FirFilter( const sc_module_name& nm ):
     sdr_module::Module<filter::INPUT_TYPE,filter::OUTPUT_TYPE>( nm ), 
     initialized_( false ) { }

  void LoadCoefficients( const filter::coeff_list& coeff ) { 
      coeff_ = coeff;
      queue_.resize( coeff_.size() );
      coeff_iter_ = coeff_.begin();
      initialized_ = true;
   }

};

#endif
