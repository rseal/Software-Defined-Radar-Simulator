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
