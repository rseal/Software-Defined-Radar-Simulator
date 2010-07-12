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

#include <systemc.h>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <tr1/math.h>
#include <fstream>
#include "test_bench.hpp"

// use global constants in test_bench namespace
using test_bench::bit_type;
using test_bench::data_type;
using test_bench::z_type;
using test_bench::DATA_WIDTH;
using test_bench::Z_WIDTH;

class Stimulus : public sc_module
{
   // setup files to store data for display 
   typedef boost::shared_ptr<ofstream> OutputFileStream;
   OutputFileStream zFile_;
   OutputFileStream xFile_;
   OutputFileStream yFile_;

   // define constants
   const int RESET_TIME_;
   const string TITLE_;
   const string VERSION_;
   const string AUTHOR_;
   const float TWO_PI_;
   int errors_;
   int t;

   sc_int<test_bench::SAMPLE_WIDTH> sig_out;
   sc_int<test_bench::THETA_WIDTH> theta;

   // Reset signal
   void Reset()
   {
      t=0;
      reset = 1;
      wait(RESET_TIME_, SC_NS);
      reset = 0;
      //cout << "RESET cleared at " << sc_time_stamp() << endl;
   }

   // Data input - executed once per clock edge
   void Data()
   {
      // TODO: Move these to file for run-time simulation options
      const double f = 20.0e6;
      const double fddc = -19.609375e6;
      const double Fs = 64e6;

      const double SCALE = tr1::pow(2.0,test_bench::THETA_WIDTH);
      static sc_int<test_bench::SAMPLE_WIDTH> x_sig = 0;

      int x_amp = tr1::pow(2.0,test_bench::SAMPLE_WIDTH-1)*0.75;

      if(!reset.read()) {

         theta += rint(SCALE*fddc/Fs);
         sig_out = x_amp*tr1::cos(TWO_PI_*f/Fs * t++);

         // 12-bit real signal from the ADCs
         x_out.write( sig_out );
         y_out.write(0);

         // take upper 16 bits from 32-bit phase accumulator
         z_out = z_type(theta.range(test_bench::THETA_WIDTH-1,test_bench::THETA_WIDTH - test_bench::Z_WIDTH));

         // write variables to file
         *xFile_ << x_in << endl;
         *yFile_ << y_in << endl;
         *zFile_ << z_in << endl;

         //cout << sig_out << endl;
      }
      else
      {
         x_out = 0;
         y_out = 0;
         z_out = 0;
      }

   }

   // Display used for debugging 
   void Display() { 
      cout 
         << "x= " 
         << x_in.read() 
         << " y= " 
         << y_in.read() 
         << " z= " 
         << z_in.read()
         << " angle = " 
         << tr1::atan2((double)y_in.read(),(double)x_in.read())*180/3.14 
         << endl;
   }

   public:
   SC_HAS_PROCESS(Stimulus);

   // CTOR
   Stimulus(sc_module_name nm, const double simulationTime ):
      sc_module(nm), 
      RESET_TIME_(10),
      TITLE_("CORDIC_SHIFT_ADDER Stimulus Module"),
      VERSION_("1.0"),
      AUTHOR_("RSeal"),
      TWO_PI_(2.0f*boost::math::constants::pi<float>()),
      errors_(0)
   {
      // reset method to initialize system
      SC_THREAD( Reset );
      sensitive << clock.pos();

      // load data on negative clock edge
      SC_METHOD( Data );
      sensitive << clock.pos();

      // initialize file streams
      xFile_ = OutputFileStream( new ofstream("xout.dat",ios::out));
      yFile_ = OutputFileStream( new ofstream("yout.dat",ios::out));
      zFile_ = OutputFileStream( new ofstream("zout.dat",ios::out));
   }

   // define ports
   sc_out<bit_type> reset;
   sc_in_clk clock;
   sc_out<data_type> x_out, y_out;
   sc_out<z_type> z_out;
   sc_in <data_type> x_in, y_in;
   sc_in <z_type> z_in;

   // write summary on exit
   ~Stimulus()
   {
      //cout << "\n\n\n" << TITLE_ << "    Version " << VERSION_ << "   " << AUTHOR_ << "\n" << endl;

      //if(!errors_)
      //   cout << "All test cases passed" << endl;
      //else
      //   cout << "Found " + boost::lexical_cast<string>(errors_) + " errors " << endl;
   }
};

#endif
