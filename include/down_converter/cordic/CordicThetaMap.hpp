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
#ifndef CORDIC_THETA_MAP_HPP
#define CORDIC_THETA_MAP_HPP

#include <systemc.h>

template < uint INPUT_WIDTH, uint OUTPUT_WIDTH, 
         uint Z_INPUT_WIDTH, uint Z_OUTPUT_WIDTH >
class CordicThetaMap : public sc_module
{
   typedef sc_int<INPUT_WIDTH> cordic_input_type; 
   typedef sc_int<OUTPUT_WIDTH> cordic_output_type; 
   typedef sc_uint<Z_INPUT_WIDTH> cordic_z_input_type;
   typedef sc_int<Z_OUTPUT_WIDTH> cordic_z_output_type;

   void Compute()
   {
      cordic_z_input_type z_buff = cordic_z_input_type( zin.read() );

      // xor bits to determine quadrant - tested
      int sign = ( z_buff[Z_INPUT_WIDTH-1] ^ z_buff[Z_INPUT_WIDTH-2] ) ? -1 : 1;

      // flip sign if x,y fall in quadrant 2 or 3 - tested
      xout = cordic_output_type( sign*xin.read() );
      yout = cordic_output_type( sign*yin.read() );

      // trim one bit (MSB) - tested 05/29/2010 with 38229 --> 5461.
      int result = z_buff.range(Z_INPUT_WIDTH-2,0);

      zout = cordic_z_output_type( result );
   }

   public:

   SC_HAS_PROCESS(CordicThetaMap);

   CordicThetaMap( const sc_module_name& nm): sc_module(nm)
   {
      SC_METHOD(Compute);
      sensitive <<  clock.pos();
   }

   sc_in_clk clock;
   sc_in<cordic_input_type> xin;
   sc_in<cordic_input_type> yin;
   sc_in<cordic_z_input_type> zin;
   sc_out<cordic_output_type> xout;
   sc_out<cordic_output_type> yout;
   sc_out<cordic_z_output_type> zout;
};

#endif
