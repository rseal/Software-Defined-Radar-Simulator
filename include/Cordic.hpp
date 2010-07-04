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
#include <fstream>

#include <sdr_simulator/CordicStage.hpp>
#include <sdr_simulator/CordicThetaMap.hpp>

using std::ofstream;

template<uint XY_WIDTH, uint Z_WIDTH>
class Cordic: public sc_module
{
   // define constants
   const unsigned int NUM_STAGES;

   // data type definitions 
   typedef bool bit_type;

   typedef sc_int<XY_WIDTH> data_type;
   typedef sc_int<Z_WIDTH> z_type;

   typedef sc_int<XY_WIDTH+2> int_data_type;
   typedef sc_int<Z_WIDTH-1> int_z_type;

   // data signal definitions 
   typedef sc_signal<data_type> data_signal;
   typedef sc_signal<z_type> z_signal;
   typedef sc_signal<int_data_type> int_data_signal;
   typedef sc_signal<int_z_type> int_z_signal;

   // define shared_ptrs to manage module and signal containers
   typedef CordicStage< XY_WIDTH+2, Z_WIDTH-1> CordicStageModule;

   typedef boost::shared_ptr< CordicStageModule > CordicStagePtr;
   typedef boost::shared_ptr<int_data_signal> DataSignalPtr; 
   typedef boost::shared_ptr<int_z_signal> ZSignalPtr; 

   // define internal modules
   vector<CordicStagePtr> cordicStages_;

   // define internal signals
   vector<DataSignalPtr> xSignals_;
   vector<DataSignalPtr> ySignals_;
   vector<ZSignalPtr> zSignals_;

   typedef CordicThetaMap< XY_WIDTH, XY_WIDTH+2, Z_WIDTH, Z_WIDTH-1> theta_map;
   typedef boost::shared_ptr<theta_map> ThetaMapPtr;
   ThetaMapPtr thetaMap_;

   int_data_signal x_theta;
   int_data_signal y_theta;
   int_z_signal z_theta;

   int_data_signal xout_buff;
   int_data_signal yout_buff;
   int_z_signal zout_buff;

   typedef boost::shared_ptr<ofstream> OutputFileStreamPtr;
   OutputFileStreamPtr outputStream_;

   // create internal shift adder modules
   void InitializeModules()
   {
      string stageNumber;

      // create N-1 stages and signals 
      for( int i=0; i<NUM_STAGES; ++i)
      {
         stageNumber = boost::lexical_cast<string>(i);

         cordicStages_.push_back ( 
               // add 2 bits to data width and reduce z width by 1 bit
               CordicStagePtr ( new CordicStageModule( stageNumber.c_str(), i ) )
               );

      }

      for( int i=0; i<NUM_STAGES; ++i)
      {
         CordicStagePtr current_stage = cordicStages_[i];
         current_stage->reset(reset);
         current_stage->clock(clock);
      }

      for( int i=0; i<NUM_STAGES-1; ++i)
      {
         xSignals_.push_back( DataSignalPtr( new int_data_signal ) );
         ySignals_.push_back( DataSignalPtr( new int_data_signal ) );
         zSignals_.push_back( ZSignalPtr( new int_z_signal ) );
      }

      // create interconnect signals for Cordic_stages 
      for( int i=0; i<NUM_STAGES-1; ++i)
      {
         CordicStagePtr output_stage = cordicStages_[i];
         CordicStagePtr input_stage = cordicStages_[i+1];
         DataSignalPtr x = xSignals_[i];
         DataSignalPtr y = ySignals_[i];
         ZSignalPtr z = zSignals_[i];

         // connect signal to outputs of stage i
         output_stage->xout(*x);
         output_stage->yout(*y);
         output_stage->zout(*z);

         // connect signal to inputs of stage i+1
         input_stage->xin(*x);
         input_stage->yin(*y);
         input_stage->zin(*z);
      }

      thetaMap_ = ThetaMapPtr( new theta_map("theta_map") );
      thetaMap_->xin(xin);
      thetaMap_->yin(yin);
      thetaMap_->zin(zin);
      thetaMap_->xout(x_theta);
      thetaMap_->yout(y_theta);
      thetaMap_->zout(z_theta);
      thetaMap_->clock(clock);

      //// connect Cordic input ports to first Cordic_stage
      CordicStagePtr first_stage = cordicStages_[0];
      first_stage->xin(x_theta);
      first_stage->yin(y_theta);
      first_stage->zin(z_theta);

      //// connect Cordic output ports to last Cordic_stage
      CordicStagePtr last_stage = cordicStages_[NUM_STAGES-1];
      last_stage->xout(xout_buff);
      last_stage->yout(yout_buff);
      last_stage->zout(zout_buff);
   }

   void Compute()
   {
      int_data_type x_buff,y_buff;
      z_type z_buff;

      // convert 18-bit --> 16-bit
      x_buff = xout_buff;
      y_buff = yout_buff;
      z_buff = zout_buff;

      xout = x_buff.range(XY_WIDTH+1,1);
      yout = y_buff.range(XY_WIDTH+1,1);
      zout = z_buff;

   }

   public:

   //required by systemc when not using CTOR macros
   SC_HAS_PROCESS(Cordic);

   //CTOR
   Cordic(sc_module_name nm, const int& num_stages): 
      sc_module(nm), NUM_STAGES(num_stages)
   {
      // setup internal modules
      InitializeModules();

      SC_METHOD(Compute);
      sensitive << clock.pos();

   }

   // define ports
   sc_in_clk clock;
   sc_in<bit_type> reset;
   sc_in<data_type>  xin;
   sc_in<data_type>  yin;
   sc_in<z_type>  zin;
   sc_out<data_type> xout;
   sc_out<data_type> yout;
   sc_out<z_type> zout;
};

#endif
