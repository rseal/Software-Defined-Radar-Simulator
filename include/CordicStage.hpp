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
#ifndef CORDICSTAGE_HPP
#define CORDICSTAGE_HPP

#include <systemc.h>
#include <boost/shared_ptr.hpp>
#include <boost/math/constants/constants.hpp>
#include <tr1/math.h>
#include <vector>
#include <iomanip>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <sdr_simulator/CordicShiftAdder.hpp> 

template<uint XY_WIDTH, uint Z_WIDTH>
class CordicStage: public sc_module
{
   typedef boost::shared_ptr<ofstream> OutputFileStream;
   OutputFileStream ofStream_;

   std::vector<int> atan_vector_;

   // internal typedefs
   typedef boost::shared_ptr< CordicShiftAdder< XY_WIDTH, XY_WIDTH > > ShiftAdderPtr;
   typedef bool reset_type;
   typedef sc_uint<1>  bit_type;

   // define internal modules 
   ShiftAdderPtr shiftAdder1;
   ShiftAdderPtr shiftAdder2;

   // define internal signals
   sc_signal< bit_type > sign_;
   sc_signal< bit_type > not_sign_;

   // define constants
   const unsigned int STAGE;
   
   // create internal shift adder modules
   void InitializeModules()
   {
      // create first shift adder object
      shiftAdder1 = ShiftAdderPtr( new CordicShiftAdder<XY_WIDTH,XY_WIDTH>("shift_adder_1",STAGE ));
      shiftAdder1->sign(not_sign_);
      shiftAdder1->reset(reset);
      shiftAdder1->clock(clock);
      shiftAdder1->in_a(xin);
      shiftAdder1->in_b(yin);
      shiftAdder1->out(xout);

      // create second shift adder object
      shiftAdder2 = ShiftAdderPtr( new CordicShiftAdder<XY_WIDTH,XY_WIDTH>("shift_adder_2",STAGE ) );
      shiftAdder2->sign(sign_);
      shiftAdder2->reset(reset);
      shiftAdder2->clock(clock);
      shiftAdder2->in_a(yin);
      shiftAdder2->in_b(xin);
      shiftAdder2->out(yout);

      // setup arctangent constants - 16 stages max
      atan_vector_.push_back(8192);
      atan_vector_.push_back(4836);
      atan_vector_.push_back(2555);
      atan_vector_.push_back(1297);
      atan_vector_.push_back(651);
      atan_vector_.push_back(326);
      atan_vector_.push_back(163);
      atan_vector_.push_back(81);
      atan_vector_.push_back(41);
      atan_vector_.push_back(20);
      atan_vector_.push_back(10);
      atan_vector_.push_back(5);
      atan_vector_.push_back(3);
      atan_vector_.push_back(1);
      atan_vector_.push_back(1);
      atan_vector_.push_back(0);
      atan_vector_.push_back(0);
   }

   // compute performance variable Z
   void Z()
   {
      if(!reset.read()){
         // read current error
         sc_int<Z_WIDTH> z = zin.read();
      
         bit_type sign_bit = static_cast<bit_type>(z[Z_WIDTH-1]);
         int sign = sign_bit == 0 ? 1 : -1;

         // assign internal sign variable
         sign_ = sign_bit;
         not_sign_ = ~sign_bit;

         // compute error 
         zout = z - sign*atan_vector_[STAGE];
      }
      else{
         sign_ = 0;
         not_sign_ = 0;
         zout = 0;
      }

      //Display();
   }

   void Display() {
      if( !reset.read() ) {
         *ofStream_ 
            << std::setw(12) << this->name()   
            << " x_in = " << std::setw(6) << xin.read() 
            << " y_in = " << std::setw(6) << yin.read() 
            << " z_in = " << std::setw(6) << zin.read() 
            << " x_out = " << std::setw(6) << xout.read() 
            << " y_out = " << std::setw(6) << yout.read() 
            << " z_out = " << std::setw(6) << zout.read() 
            << " at " << std::setw(6) << sc_time_stamp() << endl;
      }
   }

   public:

   //required by systemc when not using CTOR macros
   SC_HAS_PROCESS(CordicStage);

   //CTOR
   CordicStage(sc_module_name nm, const int& stage): 
      sc_module(nm), STAGE(stage)
   {
      // setup internal modules
      InitializeModules();

      // define clock sensitive methods
      SC_METHOD(Z);
      sensitive << zin << reset;

      //string name = this->name();
      //name += ".dat";
      //ofStream_ = OutputFileStream( new ofstream( name.c_str(), ios::out ) );
   }

   // define data types for ports
   typedef sc_int<XY_WIDTH> data_type;
   typedef sc_int<Z_WIDTH> z_type;

   //define interface to module
   sc_in_clk clock;
   sc_in< reset_type> reset;
   sc_in<data_type>  xin;
   sc_in<data_type>  yin;
   sc_in<z_type>  zin;
   sc_out<data_type> xout;
   sc_out<data_type> yout;
   sc_out<z_type> zout;
};

#endif
