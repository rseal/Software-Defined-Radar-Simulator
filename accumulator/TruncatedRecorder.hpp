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
#ifndef TRUNCATED_RECORDER_HPP
#define TRUNCATED_RECORDER_HPP

#include <systemc.h>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <sdr_simulator/util/Recorder.hpp>

template < typename T >
class TruncatedRecorder: public Recorder<T>
{
   // define output file stream for data file recording.
   typedef boost::shared_ptr<std::ofstream> OutputFileStreamPtr;
   OutputFileStreamPtr outputFileStream;

   // override method to write data to the outputstream
   virtual void Log() {

      T buffer = this->input.read();

      // grab the upper 16 bits of the accumulator output
      sc_int<16> truncBuffer = buffer.range(31,16);

      *outputFileStream << truncBuffer << "\n";

   }

   public:

      SC_HAS_PROCESS( TruncatedRecorder );

      // CTOR
      TruncatedRecorder( const sc_module_name& nm, const std::string& fileName ): 
         Recorder<T>( nm ) {

         outputFileStream = OutputFileStreamPtr( 
               new ofstream( fileName.c_str() ) 
               );
      }
};

#endif
