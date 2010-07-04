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
#include <systemc.h>
#include <boost/lexical_cast.hpp>
#include "test_bench.hpp"

using test_bench::reset_type;
using test_bench::data_type;
using test_bench::z_type;
using test_bench::DATA_WIDTH;
using test_bench::Z_WIDTH;

class Stimulus : public sc_module
{
   // define constants
   const int RESET_TIME_;
   const int DATA_TIME_;
   const int SWITCH_TIME_;
   const string TITLE_;
   const string VERSION_;
   const string AUTHOR_;

   int errors_;

   // Reset bit signal
   void Reset()
   {
      reset.write( true );
      wait(RESET_TIME_, SC_NS);
      reset.write( false );
   }

   // Data input - executed once per clock edge
   void Data()
   {
      static int x_data = SWITCH_TIME_*2;
      static int y_data = SWITCH_TIME_*2;
      static int z_data = SWITCH_TIME_/2;

      x_out = x_data--;
      y_out = y_data--;
      z_out = z_data--;

      Display();
   }

   void Display( ) {
      cout << "x= " << x_in.read() << " y=" << y_in.read() << " z=" << z_in.read() << endl;
      cout << "x_out= " << x_out.read() << " y_out=" << y_out.read() << " z_out=" << z_out.read() << endl;
   }

   public:
      SC_HAS_PROCESS(Stimulus);

      Stimulus(sc_module_name nm, const double simulationTime ):
         sc_module(nm), 
         RESET_TIME_(10),
         DATA_TIME_(1),
         SWITCH_TIME_(simulationTime/4),
         TITLE_("CORDIC_SHIFT_ADDER Stimulus Module"),
         VERSION_("1.0"),
         AUTHOR_("RSeal"),
         errors_(0),
         SHIFT(1.0)
      {
         SC_THREAD( Reset );
         sensitive << clock.pos();

         SC_METHOD( Data );
         sensitive << clock.neg();

      }

      sc_out< reset_type > reset;
      sc_in_clk clock;
      sc_out<data_type> x_out,y_out;
      sc_out<z_type> z_out;
      sc_in <data_type> x_in;
      sc_in <data_type> y_in;
      sc_in <z_type> z_in;

      const int SHIFT;

      // write summary on exit
      ~Stimulus()
      {
         cout << "\n\n\n" << TITLE_ << "    Version " << VERSION_ << "   " << AUTHOR_ << "\n" << endl;

         if(!errors_)
            cout << "All test cases passed" << endl;
         else
            cout << "Found " + boost::lexical_cast<string>(errors_) + " errors " << endl;
      }
};
