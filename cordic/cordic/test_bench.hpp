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

namespace test_bench{

   const int DATA_WIDTH=16;
   const int Z_WIDTH=16;

   typedef sc_uint<1> bit_type;
   typedef sc_int<DATA_WIDTH> data_type;
   typedef sc_int<Z_WIDTH> z_type;
};

#endif
