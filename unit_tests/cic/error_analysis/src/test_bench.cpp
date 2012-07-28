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
#include <sdr_simulator/filter/cic/CicErrorAnalysis.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

using namespace std;
using boost::lexical_cast;

int main()
{
   const int NUM_STAGES     = 4;
   const int MAX_DECIMATION = 128;
   const int DIFF_DELAY     = 1;
   const int INPUT_WIDTH    = 16;
   const int OUTPUT_WIDTH   = 16;

   CicErrorAnalysis cic_error_analysis;
   vector<int> bitWidths = cic_error_analysis.ComputeBitWidths( 
         NUM_STAGES, MAX_DECIMATION, DIFF_DELAY, INPUT_WIDTH, OUTPUT_WIDTH
         );
  
   for( unsigned int i=0; i< bitWidths.size(); ++i)
   {
      cout << " stage " << lexical_cast<string>(i) << " bit width = " << bitWidths[i] << endl;
   }

   return 0;
}
