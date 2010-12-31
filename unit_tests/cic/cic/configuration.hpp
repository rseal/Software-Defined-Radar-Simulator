#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <systemc>
#include <list>
#include <tr1/math.h>

namespace cic
{
   const int DATA_WIDTH= 52;
   const int PHASE_WIDTH = 52;
   const int NUM_STAGES = 4;
   typedef sc_int<DATA_WIDTH> INPUT_TYPE;
   typedef sc_int<DATA_WIDTH> OUTPUT_TYPE;
   typedef bool RESET_TYPE;
};

#endif
