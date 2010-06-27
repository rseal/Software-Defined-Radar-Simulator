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

// Stimulus module for cordic_shift_adder
template< uint DATA_WIDTH>
class Stimulus : public sc_module
{
   // define signal types
   typedef sc_uint<1> BitType;
   typedef sc_int<DATA_WIDTH> DataType;

   // define constants
   const int RESET_TIME_;
   const int SIGN_TIME_;
   const int DATA_TIME_;
   const int SWITCH_TIME_;
   const string TITLE_;
   const string VERSION_;
   const string AUTHOR_;

   int errors_;

   // Sign bit signal 
   void Sign()
   {
      static int time=0;
      
      if(time == 0) sign = 0; 
      else if(time == SWITCH_TIME_) sign = 1;

      ++time;
   }

   // Reset bit signal
   void Reset()
   {
      reset = 1;
      wait(RESET_TIME_, SC_NS);
      reset = 0;
   }

   // Data input - executed once per clock edge
   void Data()
   {
      static int x_data = SWITCH_TIME_*2;
      static int y_data = SWITCH_TIME_*2;

      int sign_bit = sign.read() == 0 ? 1 : -1;

      // test expected vs. actual and report errors
      if( sc_time_stamp() > sc_time(RESET_TIME_, SC_NS)) {

         // handle case where sign switches state
         const double SIGN_SWITCH_EXCEPTION_TIME = 2*SWITCH_TIME_-1;

         // expected value 
         int expected = x.read()+sign_bit*(y.read()>> SHIFT);

         // test for error
         bool error = 
            expected != out.read() and 
            sc_time_stamp() != sc_time( SIGN_SWITCH_EXCEPTION_TIME , SC_NS );

         // record and report if error
         if (error) {
            ++errors_;
            Display(expected); 
         }
      }

      x = x_data--;
      y = y_data--;

   }

   void Display(const int& value)
   {
      cout 
         << "ERROR at " << sc_time_stamp() << "\n"
         << "expected " << value << " received " << out.read() << "\n"
         << "sign = " << sign.read() << "  "
         << "   x = " << x.read() << "  "
         << "   y = " << y.read() << endl;
   }

   public:
      SC_HAS_PROCESS(Stimulus);

      Stimulus(sc_module_name nm, const double simulationTime ):
         sc_module(nm), 
         RESET_TIME_(10),
         SIGN_TIME_(20),
         DATA_TIME_(1),
         SWITCH_TIME_(simulationTime/4),
         SHIFT(1.0),
         VERSION_("1.0"),
         AUTHOR_("RSeal"),
         TITLE_("CORDIC_SHIFT_ADDER Stimulus Module"),
         errors_(0)
      {
         SC_THREAD( Reset );
         sensitive << clock.pos();

         SC_METHOD( Sign );
         sensitive << clock.pos();
         
         SC_METHOD( Data );
         sensitive << clock.neg();
      }

      sc_out<BitType> reset;
      sc_out<BitType> sign;
      sc_in_clk clock;
      sc_out<DataType> x,y;
      sc_in <DataType> out;

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
