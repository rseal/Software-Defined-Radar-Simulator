#ifndef TYPES_HPP
#define TYPES_HPP

#include<systemc.h>

// commonly used data/signal types
//
namespace sdr_types {

typedef sc_uint<1> bit_type;
typedef bool reset_type;

typedef sc_signal< bit_type > bit_signal;
typedef sc_signal< reset_type > reset_signal;

typedef sc_export< sc_signal_inout_if< bool > > clk_export_out;
typedef sc_export< sc_signal_inout_if< reset_type > > reset_export_out;
};

#endif
