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
#ifndef STIMULUS_HPP
#define STIMULUS_HPP

#include "test_bench.hpp"

#include <systemc.h>
#include <tr1/math.h>
#include <fstream>
#include <boost/math/constants/constants.hpp>
#include <boost/shared_ptr.hpp>

class Stimulus: public sc_module
{
   typedef boost::shared_ptr<std::ofstream> OutputFileStreamPtr;
   OutputFileStreamPtr outputFileStream;

   const unsigned int RESET_TIME_;

   // define an exportable clock signal
   typedef sc_export< sc_signal_inout_if<bool> > clk_out;
   sc_clock clock_;

   void Reset() {
      reset = 1;
      wait(RESET_TIME_, SC_NS);
      reset = 0;
   }

   void Data() {
      static const float TWO_PI = 2.0f*boost::math::constants::pi<float>();
      static const float sampleRate = 200e6;
      static float frequency = 0;
      static const float theta = TWO_PI * frequency/sampleRate; 
      static int t = 0;

      if( reset.read() )
      {
         output = 0;
         t = 0;
      }
      else
      {
         output = std::tr1::cos(TWO_PI*frequency/sampleRate*t)*16000;
         cout << "stimulus = " << output.read() << endl;
         ++t;
         frequency += 100;
         *outputFileStream << output << "\n";
      }
   }

   public:
      SC_HAS_PROCESS( Stimulus );

      Stimulus( const sc_module_name nm): 
         sc_module(nm), 
         RESET_TIME_(10),
         clock_( "clock", sc_time( 2, SC_NS) ) {

         SC_THREAD( Reset );
         sensitive << clock_.posedge_event();

         SC_METHOD( Data );
         sensitive << clock_.posedge_event();

         clock(clock_);

         outputFileStream = OutputFileStreamPtr( new ofstream( "stimulus.dat" ) );
      }

      sc_out< testbench::bit_type > reset;
      clk_out clock;
      sc_out< testbench::data_input_type > output;
};

#endif
