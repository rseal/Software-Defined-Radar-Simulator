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
#ifndef FILTER_STAGE_HPP
#define FILTER_STAGE_HPP

#include<fstream>
#include<systemc.h>
#include<boost/shared_ptr.hpp>

#include<sdr_simulator/filter/fir/FirFilter.hpp>

// locally-generated files.
#include "configuration.hpp"

namespace usrp_v1
{
	class FilterStage;
   typedef boost::shared_ptr< FilterStage > FilterStagePtr;

   class FilterStage: public sdr_module::Module< filter::INPUT_TYPE, filter::OUTPUT_TYPE >
   {
      typedef sc_export< sc_signal_inout_if<bool> > sc_export_clk;

		filter::CicPtr cic_;

      // signal and data type definitions
      sc_signal< filter::INPUT_TYPE > internal_signal_1;

      void Compute() 
      {
         this->output.write( internal_signal_1.read() );
      }

      public:

      SC_HAS_PROCESS( FilterStage );

      // CTOR
      FilterStage( const sc_module_name& nm ): sdr_module::Module< filter::INPUT_TYPE, filter::OUTPUT_TYPE >(nm)
      {

         cic_ = filter::CicPtr( new filter::CicFilter( "cic" ) );
         cic_->clock( this->clock );
         cic_->reset( this->reset );
         cic_->decimation( decimation );
         cic_->input( this->input );
         cic_->output( internal_signal_1 );

         div_clock( cic_->div_clock );

         sensitive << cic_->div_clock->posedge_event();
      }

      sc_in< filter::DECIMATION_TYPE > decimation;
      sc_export_clk div_clock; 
      //sc_export_clk cic_output_clock;
   };
};

#endif
