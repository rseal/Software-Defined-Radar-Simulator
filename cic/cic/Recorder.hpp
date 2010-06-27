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
#ifndef RECORDER_HPP
#define RECORDER_HPP

#include <systemc.h>
#include <iostream>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include "test_bench.hpp"

class Recorder: public sc_module
{
   typedef boost::shared_ptr<std::ofstream> OutputFileStreamPtr;
   OutputFileStreamPtr outputFileStream;

   void Log() {
      *outputFileStream << input.read() << "\n";
      //std::cout << input.read() << " at " << sc_time_stamp() << std::endl;
   }


   public:

      SC_HAS_PROCESS( Recorder );

      Recorder( const sc_module_name& nm ): sc_module(nm){

         outputFileStream = OutputFileStreamPtr( new ofstream("output.dat") );

         SC_METHOD( Log );
         sensitive << input;

      }

      sc_in< testbench::data_input_type > input;
      sc_in_clk clock;
};

#endif
