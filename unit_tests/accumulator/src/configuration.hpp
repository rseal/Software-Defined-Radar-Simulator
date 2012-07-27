#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <systemc>
#include <list>
#include <tr1/math.h>

namespace accumulator
{
   const unsigned int DATA_WIDTH = 16;
   const double SAMPLE_RATE = 64e6;
   const double TUNING_FREQUENCY = -14.2e6;
   typedef sc_int< DATA_WIDTH > DATA_TYPE;

   // static helper function
   static const sc_int< DATA_WIDTH> StepSize() { 
      return rint( std::tr1::pow(2.0,DATA_WIDTH) * TUNING_FREQUENCY / SAMPLE_RATE );
   }
};
   
#endif
