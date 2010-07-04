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

// abstract class - override Log method in your own class 
template < typename T >
class Recorder: public sc_module
{
   // pure virtual method 
   virtual void Log()=0;

   public:

      SC_HAS_PROCESS( Recorder );

      // CTOR
      Recorder( const sc_module_name& nm ): sc_module(nm){

         SC_METHOD( Log );
         sensitive << input;
      }

      // define Port IO
      sc_in< T > input;
      sc_in_clk clock;
};

#endif
