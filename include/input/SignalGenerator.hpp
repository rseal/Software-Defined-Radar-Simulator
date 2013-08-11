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

#include <sdr_simulator/SdrModule.hpp>

#include<vector>

template<typename DATA_TYPE, typename RESET_TYPE>
class SignalGenerator : public sdr_module::Module< DATA_TYPE, DATA_TYPE >
{
   unsigned int index_;
   sc_signal< DATA_TYPE > null_input_signal_;

   // default implementation that relies on samples_ vector, which is filled 
   // by implementing GenerateSamples.
   virtual void Compute()
   {
		if( this->reset.read() || index_ == SAMPLE_SIZE )
		{
			index_=0;
			this->output = 0;
		}
		else
		{
			this->output = samples_[index_];
			++index_;
		}
   }

   // Optional method for providing a fixed number of random samples
   virtual void GenerateSamples() {};

   protected:

   typedef std::vector< DATA_TYPE > SampleVector;
   const int SAMPLE_SIZE;
   SampleVector samples_;

   public:

   SC_HAS_PROCESS( SignalGenerator );

   // CTOR: Set sampleSize to zero if overriding the Compute method.
   SignalGenerator( const sc_module_name& nm, const int sampleSize):
      sdr_module::Module<DATA_TYPE, DATA_TYPE>( nm ), 
      SAMPLE_SIZE( sampleSize ), samples_(sampleSize, DATA_TYPE()), index_(0)
   {
      this->input( null_input_signal_ );
   }

   // must call this prior to use - if using a fixed vector generated data. 
   // A good example of this would be a sine table.
   void Init(){
      GenerateSamples();
   }
};

#endif
