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
#ifndef SIGNAL_GENERATOR_HPP
#define SIGNAL_GENERATOR_HPP

#include<systemc.h>
#include<vector>
#include<sdr_simulator/Types.hpp>

template< unsigned int BIT_WIDTH >
class SignalGenerator : public sc_module
{
   unsigned int index_;

   // default implementation that relies on samples_ vector, which is filled 
   // by implementing GenerateSamples.
   virtual void Compute()
   {
     if( index_ == samples_.size() || reset.read() ) index_ = 0;
     output = samples_[index_++];
   }

   // Optional method for providing a fixed number of random samples
   virtual void GenerateSamples() {};

   protected:

   typedef sc_int< BIT_WIDTH > sample_type;
   const int SAMPLE_SIZE;
   std::vector< sample_type > samples_;

   public:

   SC_HAS_PROCESS( SignalGenerator );

   // CTOR: Set sampleSize to zero if overriding the Compute method.
   SignalGenerator( const sc_module_name& nm, const int sampleSize):
      sc_module( nm ), SAMPLE_SIZE( sampleSize ), samples_(sampleSize), index_(0)
   {
      // virtual method is sensitive to clock edge. A custom implementation 
      // should be provided to override Compute, producing a single sample 
      // output per clock cycle. See GaussianNoiseGenerator.hpp
      SC_METHOD( Compute );
      sensitive << clock.pos();
   }

   // define port IO
   sc_in< sdr_types::reset_type > reset;
   sc_in_clk clock;
   sc_out< sample_type > output;

   // must call this prior to use - if using a fixed vector generated data. 
   // A good example of this would be a sine table.
   void Init(){
      GenerateSamples();
   }
};

#endif
