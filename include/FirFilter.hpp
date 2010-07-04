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

#include <systemc.h>
#include <list>
#include <deque>

template < unsigned int INPUT_WIDTH, unsigned int OUTPUT_WIDTH, 
         unsigned int COEFF_WIDTH, unsigned int SUM_WIDTH> 
         class FirFilter: public sc_module {

   // data type definitions
   typedef sc_uint<1> bit_type;
   typedef sc_int< INPUT_WIDTH > data_input_type;
   typedef sc_int< OUTPUT_WIDTH > data_output_type;
   typedef sc_int< COEFF_WIDTH > coeff_type;
   typedef std::list< coeff_type > CoeffList;
   typedef std::deque< coeff_type > CoeffDeque;
   typedef typename CoeffList::iterator CoeffIterator;
   CoeffList coeff_;
   CoeffDeque queue_;
   CoeffIterator coeff_iter_;

   // default implementation
   virtual void Compute() {

      static bool toggle = true;

      sc_int< SUM_WIDTH > sum = 0.0;

      // delete the oldest sample
      queue_.pop_front();
      // add the latest sample
      queue_.push_back( input.read() );

      // compute convolution sum
      coeff_iter_ = coeff_.end();
      for( int i=0; i<queue_.size(); ++i) {
         sum += queue_[i]* *(--coeff_iter_);
      }

      // compute output
      output = sum.range(SUM_WIDTH-1,SUM_WIDTH-OUTPUT_WIDTH);
   }

   public:

   SC_HAS_PROCESS( FirFilter );

   // CTOR
   FirFilter( const sc_module_name& nm, const CoeffList& coeff ):
   sc_module( nm ), coeff_(coeff), queue_(coeff.size(),0) {

      SC_METHOD( Compute );
      sensitive << clock.pos();
      
      // initialize coefficient list iterator
      coeff_iter_ = coeff_.begin();
   }

   // port IO definitions
   sc_in_clk clock;
   sc_in< bit_type > reset;
   sc_in< data_input_type > input;
   sc_out< data_output_type > output;

};

#endif
