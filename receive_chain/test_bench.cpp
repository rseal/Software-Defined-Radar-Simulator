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
#include <sdr_simulator/ReceiveChain.hpp>
#include <systemc.h>

int main (void) {

   const unsigned int INPUT_WIDTH = 16;
   const unsigned int OUTPUT_WIDTH = 16;

   typedef sc_int< INPUT_WIDTH > data_input_type;
   typedef sc_int< OUTPUT_WIDTH > data_output_type;
   typedef sc_uint<1> bit_type;

   sc_signal < data_input_type > input;
   sc_signal < data_output_type > output;
   sc_signal < bit_type > reset;
   sc_in_clk clock;

   ReceiveChain< INPUT_WIDTH, OUTPUT_WIDTH> receiveChain;
   receiveChain.input( input );
   receiveChain.output( output );
   receiveChain.reset( reset );
   receiveChain.clock( clock );
   

   return 0;

}
