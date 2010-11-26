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

#include "Cic.hpp"
#include<sdr_simulator/receiver/usrp/HalfBandFilterGeneric.hpp>

namespace{
   const static int COEFF_WIDTH = 16;
   const static int SUM_WIDTH = 37;
};

namespace usrp
{

template< typename INPUT_DATA_TYPE, typename OUTPUT_DATA_TYPE > 
class FilterStage: public sdr_module::Module< INPUT_DATA_TYPE, OUTPUT_DATA_TYPE>
{
    typedef sc_export< sc_signal_inout_if<bool> > sc_export_clk;
    typedef sc_int<16> DECIMATION_TYPE;
    typedef sc_int< ::COEFF_WIDTH > COEFF_TYPE;
    typedef std::list< COEFF_TYPE > COEFF_LIST;
    COEFF_LIST coeff_;

    // cic definitions
    // TODO: Read parameters from config file.
    typedef Cic< INPUT_DATA_TYPE, INPUT_DATA_TYPE > CicModule;
    typedef boost::shared_ptr< CicModule > CicPtr;
    CicPtr cic_;

    // half band filter definitions
    // TODO: Read parameters from config file.
    typedef HalfBandFilterGeneric< INPUT_DATA_TYPE, OUTPUT_DATA_TYPE,
            COEFF_LIST, ::SUM_WIDTH > HalfBandFilterModule;
    typedef boost::shared_ptr< HalfBandFilterModule > HalfBandFilterPtr;
    HalfBandFilterPtr halfBandFilter_;

    // signal and data type definitions
    sc_signal< INPUT_DATA_TYPE > internal_signal_1;
    sc_signal< OUTPUT_DATA_TYPE > internal_signal_2;

    virtual void Compute() {
        this->output.write( internal_signal_2 );
    }

public:

    SC_HAS_PROCESS( FilterStage );

    // CTOR
    FilterStage( const sc_module_name& nm ): 
       sdr_module::Module<INPUT_DATA_TYPE,OUTPUT_DATA_TYPE>(nm) {

      // poor implementation of a 31-tap half-band filter
         coeff_.push_back( -49 );
         coeff_.push_back( 0 );
         coeff_.push_back( 165 );
         coeff_.push_back( 0 );
         coeff_.push_back( -412 );
         coeff_.push_back( 0 );
         coeff_.push_back( 873 );
         coeff_.push_back( 0 );
         coeff_.push_back( -1681 );
         coeff_.push_back( 0 );
         coeff_.push_back( 3135 );
         coeff_.push_back( 0 );
         coeff_.push_back( -6282 );
         coeff_.push_back( 0 );
         coeff_.push_back( 20628 );
         coeff_.push_back( 32767 );
         coeff_.push_back( 20628 );
         coeff_.push_back( 0 );
         coeff_.push_back( -6282 );
         coeff_.push_back( 0 );
         coeff_.push_back( 3135 );
         coeff_.push_back( 0 );
         coeff_.push_back( -1681 );
         coeff_.push_back( 0 );
         coeff_.push_back( 873 );
         coeff_.push_back( 0 );
         coeff_.push_back( -412 );
         coeff_.push_back( 0 );
         coeff_.push_back( 165 );
         coeff_.push_back( 0 );
         coeff_.push_back( -49 );

        cic_ = CicPtr( new CicModule( "cic" ) );
        cic_->clock( this->clock );
        cic_->decimation( decimation );
        cic_->input( this->input );
        cic_->output( internal_signal_1 );
        cic_->reset( this->reset );

        halfBandFilter_ = HalfBandFilterPtr( new HalfBandFilterModule( "hbFilter") );

        // the halfband filter is clocked at the cic's output rate
        halfBandFilter_->clock( cic_->div_clock );
        halfBandFilter_->reset( this->reset );
        halfBandFilter_->input( internal_signal_1 );
        halfBandFilter_->output( internal_signal_2 );
        halfBandFilter_->LoadCoefficients( coeff_ );

        // export the div_clock to the user
        div_clock( halfBandFilter_->div_clock );
    }

    sc_in< DECIMATION_TYPE > decimation;
    sc_export_clk div_clock; 
};
};

#endif
