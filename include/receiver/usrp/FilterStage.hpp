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
#ifndef FILTER_STAGE_HPP
#define FILTER_STAGE_HPP

#include<fstream>
#include<systemc.h>
#include<boost/shared_ptr.hpp>

#include<sdr_simulator/receiver/usrp/HalfBandFilterGeneric.hpp>

// locally-generated files.
#include "Cic.hpp"
#include "configuration.hpp"

namespace usrp
{

class FilterStage: 
   public sdr_module::Module< filter::INPUT_TYPE, filter::OUTPUT_TYPE >
{
    typedef sc_export< sc_signal_inout_if<bool> > sc_export_clk;


    // cic definitions
    // TODO: Read parameters from config file.
    typedef boost::shared_ptr< Cic > CicPtr;
    CicPtr cic_;

    // half band filter definitions
    // TODO: Read parameters from config file.
    typedef boost::shared_ptr< HalfBandFilterGeneric > HalfBandFilterPtr;
    HalfBandFilterPtr halfBandFilter_;

    // signal and data type definitions
    sc_signal< filter::INPUT_TYPE > internal_signal_1;
    sc_signal< filter::OUTPUT_TYPE > internal_signal_2;

    virtual void Compute() {
        //this->output.write( internal_signal_2.read() );
    }

    void DecimatedCompute()
    {
       this->output.write( internal_signal_2.read() );
    }

public:

    SC_HAS_PROCESS( FilterStage );

    // CTOR
    FilterStage( const sc_module_name& nm ): 
       sdr_module::Module< filter::INPUT_TYPE, filter::OUTPUT_TYPE >(nm) {

        cic_ = CicPtr( new Cic( "cic" ) );
        cic_->clock( this->clock );
        cic_->reset( this->reset );
        cic_->decimation( decimation );
        cic_->input( this->input );
        cic_->output( internal_signal_1 );

        cic_output_clock( cic_->div_clock );

        halfBandFilter_ = HalfBandFilterPtr( new HalfBandFilterGeneric( "hbFilter") );
        halfBandFilter_->clock( cic_->div_clock );
        halfBandFilter_->reset( this->reset );
        halfBandFilter_->input( internal_signal_1 );
        halfBandFilter_->output( internal_signal_2 );
        halfBandFilter_->LoadCoefficients( filter::COEFFICIENTS );

        // export the div_clock to the user
        div_clock( halfBandFilter_->div_clock );

        // TODO: Debug only 
        debug_cic_output( internal_signal_1 );

        SC_METHOD( DecimatedCompute );
        sensitive << halfBandFilter_->div_clock->posedge_event();
    }

    sc_in< filter::DECIMATION_TYPE > decimation;
    sc_export_clk div_clock; 
    sc_export_clk cic_output_clock;

    // TODO: Debug only 
    debug::sc_export_cic_output debug_cic_output;

};
};

#endif
