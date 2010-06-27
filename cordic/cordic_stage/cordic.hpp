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
#ifndef CORDIC_HPP
#define CORDIC_HPP

#include <systemc.h>
#include <boost/shared_ptr.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/lexical_cast.hpp>
#include <tr1/math.h>

#include <sdr_simulator/cordic_stage.hpp>

template<uint XY_WIDTH, uint Z_WIDTH>
class cordic: public sc_module
{
   // internal typedefs
   typedef sc_int<XY_WIDTH> data_type;
   typedef sc_int<XY_WIDTH+2> int_data_type;
   typedef sc_int<Z_WIDTH> z_type;
   typedef sc_int<Z_WIDTH-1> int_z_type;
   typedef sc_signal<data_type> data_signal;
   typedef sc_signal<int_data_type> int_data_signal;
   typedef sc_signal<z_type> z_signal;
   typedef sc_signal<int_z_type> int_z_signal;

   // define shared_ptrs to manage module and signal containers
   typedef boost::shared_ptr< cordic_stage< XY_WIDTH, Z_WIDTH > > CordicStagePtr;
   typedef boost::shared_ptr<data_signal> DataSignalPtr; 
   typedef boost::shared_ptr<z_signal> ZSignalPtr; 

   // define internal modules
   vector<CordicStagePtr> cordicStages_;

   // define internal signals
   vector<DataSignalPtr> xSignals;
   vector<DataSignalPtr> ySignals;
   vector<ZSignalPtr> zSignals;

   // define constants
   const unsigned int STAGE;
   const float SHIFT;
   const float SCALE;
   const float TWO_PI;
   const unsigned int NUM_STAGES;
   
   // create internal shift adder modules
   void InitializeModules()
   {
      string stageNumber;

      // create N-1 stages and signals 
      for( int i=0; i<NUM_STAGES; ++i)
      {
         stageNumber = lexical_cast<string>(i);
         cordicStages_.push_back 
            ( 
             // add 2 bits to data width and reduce z width by 1 bit
             new cordic_stage<XY_WIDTH+2,Z_WIDTH-1>("cordic_stage_" + stageNumber, i)
            );

         cordicStages_[i]->theta(theta);
         cordicStages_[i]->reset(reset);
         cordicStages_[i]->clock(clock);
      }

      // create interconnect signals for cordic_stages 
      for( int i=0; i<NUM_STAGES-1; ++i)
      {
         xSignals_.push_back( new int_data_signal );
         ySignals_.push_back( new int_data_signal );
         zSignals_.push_back( new int_z_signal );

         // connect signal to outputs of stage i
         cordicStages_[i]->xout(*xSignals_[i]);
         cordicStages_[i]->yout(*ySignals_[i]);
         cordicStages_[i]->zout(*zSignals_[i]);

         // connect signal to inputs of stage i+1
         cordicStages_[i+1]->xin(*xSignals_[i]);
         cordicStages_[i+1]->yin(*ySignals_[i]);
         cordicStages_[i+1]->zin(*zSignals_[i]);
      }
      
      // connect cordic input ports to first cordic_stage
      cordicStages_[0]->xin(xin);
      cordicStages_[0]->yin(yin);
      cordicStages_[0]->zin(zin);

      // connect cordic output ports to last cordic_stage
      cordicStages_[NUM_STAGES-1]->xout(xout);
      cordicStages_[NUM_STAGES-1]->yout(yout);
      cordicStages_[NUM_STAGES-1]->zout(zout);
      
   }

   public:

   //required by systemc when not using CTOR macros
   SC_HAS_PROCESS(cordic_stage);

   //CTOR
   cordic_stage(sc_module_name nm, const int& stage): 
      sc_module(nm), 
      STAGE(stage),
      SHIFT(std::tr1::pow(2.0f,-stage)),
      SCALE(std::tr1::pow(2.0f,Z_WIDTH)),
      TWO_PI(2.0f*boost::math::constants::pi<float>())
   {
      // setup internal modules
      InitializeModules();

      // define clock sensitive methods
      SC_METHOD(Z);
      sensitive << clock.pos();
   }

   //define interface to module
   sc_in_clk clock;

   // define input ports
   sc_in<data_type>  xin;
   sc_in<data_type>  yin;
   sc_in<z_type>  zin;
   sc_in<z_type> theta;
   sc_in<reset_type> reset;

   // define output ports
   sc_out<data_type> xout;
   sc_out<data_type> yout;
   sc_out<z_type> zout;
};

#endif
