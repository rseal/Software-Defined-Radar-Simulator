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

#include <sdr_simulator/SdrComplexModule.hpp>

#include <sdr_simulator/down_converter/cordic/CordicStage.hpp>
#include <sdr_simulator/down_converter/cordic/CordicThetaMap.hpp>

// locally-generated constants file.
#include "configuration.hpp"

class Cordic: 
   public sdr_module::ComplexModule< cordic::INPUT_TYPE, cordic::OUTPUT_TYPE>
{

    typedef sc_signal< cordic::OUTPUT_TYPE > DebugOutputSignal;
    DebugOutputSignal real_output_debug_signal;
    DebugOutputSignal imag_output_debug_signal;

    typedef sc_int<cordic::DATA_WIDTH> data_type;
    typedef sc_int<cordic::PHASE_WIDTH> z_type;

    typedef sc_int < cordic::DATA_WIDTH + 2 > int_data_type;
    typedef sc_int < cordic::PHASE_WIDTH - 1 > int_z_type;
    typedef sc_uint<cordic::PHASE_WIDTH> theta_input_type;

    // data signal definitions
    typedef sc_signal<data_type> data_signal;
    typedef sc_signal<z_type> z_signal;
    typedef sc_signal<int_data_type> int_data_signal;
    typedef sc_signal<int_z_type> int_z_signal;
    typedef sc_signal< theta_input_type > ThetaInputSignal;

    // define shared_ptrs to manage module and signal containers
    typedef CordicStage< int_data_type, int_z_type, cordic::RESET_TYPE > CordicStageModule;

    typedef boost::shared_ptr< CordicStageModule > CordicStagePtr;
    typedef boost::shared_ptr<int_data_signal> DataSignalPtr;
    typedef boost::shared_ptr<int_z_signal> ZSignalPtr;

    // define internal modules
    vector<CordicStagePtr> cordicStages_;

    // define internal signals
    vector<DataSignalPtr> xSignals_;
    vector<DataSignalPtr> ySignals_;
    vector<ZSignalPtr> zSignals_;

    typedef CordicThetaMap < cordic::DATA_WIDTH, cordic::DATA_WIDTH + 2, 
            cordic::PHASE_WIDTH, cordic::PHASE_WIDTH - 1 > theta_map;
    typedef boost::shared_ptr<theta_map> ThetaMapPtr;
    ThetaMapPtr thetaMap_;

    ThetaInputSignal theta_input_signal;

    int_data_signal x_theta;
    int_data_signal y_theta;
    int_z_signal z_theta;

    int_data_signal xout_buff;
    int_data_signal yout_buff;
    int_z_signal zout_buff;

    // create internal shift adder modules
    void InitializeModules() {

        string stageNumber;

        // create N-1 stages and signals
        for ( int i = 0; i < cordic::NUM_STAGES; ++i ) {
            stageNumber = boost::lexical_cast<string> ( i );

            cordicStages_.push_back (
                // add 2 bits to data width and reduce z width by 1 bit
                CordicStagePtr ( new CordicStageModule ( stageNumber.c_str(), i ) )
            );
        }

        for ( int i = 0; i < cordic::NUM_STAGES; ++i ) {
            CordicStagePtr current_stage = cordicStages_[i];
            current_stage->reset ( this->reset );
            current_stage->clock ( this->clock );
        }

        for ( int i = 0; i < cordic::NUM_STAGES - 1; ++i ) {
            xSignals_.push_back ( DataSignalPtr ( new int_data_signal ) );
            ySignals_.push_back ( DataSignalPtr ( new int_data_signal ) );
            zSignals_.push_back ( ZSignalPtr ( new int_z_signal ) );
        }

        // create interconnect signals for Cordic_stages
        for ( int i = 0; i < cordic::NUM_STAGES - 1; ++i ) {
            CordicStagePtr output_stage = cordicStages_[i];
            CordicStagePtr input_stage = cordicStages_[i+1];
            DataSignalPtr x = xSignals_[i];
            DataSignalPtr y = ySignals_[i];
            ZSignalPtr z = zSignals_[i];

            // connect signal to outputs of stage i
            output_stage->xout ( *x );
            output_stage->yout ( *y );
            output_stage->zout ( *z );

            // connect signal to inputs of stage i+1
            input_stage->xin ( *x );
            input_stage->yin ( *y );
            input_stage->zin ( *z );
        }

        thetaMap_ = ThetaMapPtr ( new theta_map ( "theta_map" ) );
        thetaMap_->xin ( this->real_input );
        thetaMap_->yin ( this->imag_input );
        thetaMap_->zin ( theta_input_signal );
        thetaMap_->xout ( x_theta );
        thetaMap_->yout ( y_theta );
        thetaMap_->zout ( z_theta );
        thetaMap_->clock ( this->clock );

        //// connect Cordic input ports to first Cordic_stage
        CordicStagePtr first_stage = cordicStages_[0];
        first_stage->xin ( x_theta );
        first_stage->yin ( y_theta );
        first_stage->zin ( z_theta );

        //// connect Cordic output ports to last Cordic_stage
        CordicStagePtr last_stage = cordicStages_[cordic::NUM_STAGES-1];
        last_stage->xout ( xout_buff );
        last_stage->yout ( yout_buff );
        last_stage->zout ( zout_buff );
    }

    virtual void Compute() {

        theta_input_signal.write ( theta_input_type ( phase_input.read() ) );

        this->real_output.write ( 
              int_data_type ( xout_buff.read() ).range ( cordic::DATA_WIDTH-1 , 0 ) 
              );

        // TODO: Debug only 
        real_output_debug_signal.write(
              int_data_type ( xout_buff.read() ).range ( cordic::DATA_WIDTH-1 , 0 ) 
              );

        this->imag_output.write ( 
              int_data_type ( yout_buff.read() ).range ( cordic::DATA_WIDTH-1 , 0 ) 
              );

        // TODO: Debug only 
        imag_output_debug_signal.write( 
              int_data_type ( yout_buff.read() ).range ( cordic::DATA_WIDTH-1 , 0 ) 
              );

        phase_output.write (  cordic::PHASE_TYPE ( zout_buff.read() ) );
    }

public:

    //required by systemc when not using CTOR macros
    SC_HAS_PROCESS ( Cordic );

    //CTOR
    Cordic ( sc_module_name nm ) :
        sdr_module::ComplexModule< cordic::INPUT_TYPE, cordic::OUTPUT_TYPE> ( nm )
   {

        // setup internal modules
        InitializeModules();
        debug_cordic_i_output( real_output_debug_signal );
        debug_cordic_q_output( imag_output_debug_signal );
    }

    sc_in< cordic::PHASE_TYPE >  phase_input;
    sc_out< cordic::PHASE_TYPE > phase_output;

    // TODO: Debug only 
    debug::sc_export_cordic_output debug_cordic_i_output;
    debug::sc_export_cordic_output debug_cordic_q_output;
};

#endif
