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

#include<systemc.h>
//include<sdr_simulator/Cic.hpp>
#include<sdr_simulator/usrp/HalfBandFilterGeneric.hpp>
#include<sdr_simulator/Types.hpp>
#include<boost/shared_ptr.hpp>

#include<fstream>

namespace usrp{

   class FilterStage: public sc_module {

      typedef std::ofstream OutputFile;
      typedef boost::shared_ptr< OutputFile > OutputFileStreamPtr;
      OutputFileStreamPtr outFile_;

      // cic definitions
      typedef Cic< 16, 16 > CicModule;
      typedef boost::shared_ptr< CicModule > CicPtr;
      CicPtr cic_;

      // half band filter definitions
      typedef HalfBandFilterGeneric< 16, 16, 16, 37 > HalfBandFilterModule;
      typedef boost::shared_ptr< HalfBandFilterModule > HalfBandFilterPtr;
      HalfBandFilterPtr halfBandFilter_;

      // signal and data type definitions
      typedef sc_int<16> data_type;
      sc_signal< data_type > data_signal;
      sc_signal< data_type > out_signal;

      void Compute()
      {
         output.write( input.read() );
         //output.write( out_signal.read() );
         //*outFile_ << input.read() << endl;
      }

      public:

      SC_HAS_PROCESS( FilterStage );

      // CTOR
      FilterStage( const sc_module_name& nm ): sc_module(nm){

         //cic_ = CicPtr( new CicModule( "cic" ) );
         //cic_->clock( clock );
         //cic_->decimation( decimation );
         //cic_->input( input );
         //cic_->output( out_signal ); 
         //cic_->reset( reset );

         SC_METHOD( Compute );
         sensitive << clock.pos();

         //halfBandFilter_ = HalfBandFilterPtr( 
         //     new HalfBandFilterModule( "hbFilter")
         //     );
         //halfBandFilter_->clock( clock );
         //halfBandFilter_->reset(reset );
         //halfBandFilter_->input( data_signal );
         //halfBandFilter_->output( out_signal );

         outFile_ = OutputFileStreamPtr( new OutputFile("sine.dat") );
      }

      sc_in< data_type > input;
      sc_out< data_type > output;
      sc_in< data_type > decimation;
      sc_in< sdr_types::reset_type > reset;
      sc_in_clk clock;

   };
};

#endif
