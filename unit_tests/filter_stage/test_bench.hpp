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
#ifndef TEST_BENCH_HPP
#define TEST_BENCH_HPP

#include <systemc.h>

typedef sc_int<16> INPUT_DATA_TYPE;
typedef sc_int<16> OUTPUT_DATA_TYPE;
typedef sc_int<16> DECIMATION_TYPE;
typedef bool RESET_TYPE;
const static int COEFF_WIDTH = 16;
const static int SUM_WIDTH = 37;
const static int DECIMATION = 8;

#endif
