#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <systemc>
#include <tr1/math.h>

namespace cordic
{
   const unsigned int DATA_WIDTH= 16;
   const unsigned int PHASE_WIDTH = 16;
   const unsigned int NUM_STAGES = 16;
   typedef sc_int<DATA_WIDTH> INPUT_TYPE;
   typedef sc_int<DATA_WIDTH> OUTPUT_TYPE;
   typedef sc_int<PHASE_WIDTH> PHASE_TYPE;
   
   typedef bool RESET_TYPE;

};

namespace accumulator
{
   const unsigned int DATA_WIDTH = 16;
   const double SAMPLE_RATE = 64e6;
   const double TUNING_FREQUENCY = -1.099435e6;
   typedef sc_int< DATA_WIDTH > DATA_TYPE;

   // static helper function
   static const sc_int< DATA_WIDTH> StepSize() { 
      return rint( std::tr1::pow(2.0,DATA_WIDTH) * TUNING_FREQUENCY / SAMPLE_RATE );
   }
};

const int ADC_WIDTH = 16;
const double NORMALIZED_FREQUENCY = 0.015625;
const double DDC_FREQUENCY = -960937.5;
const double SAMPLE_RATE = 64000000;
const int ACCUMULATOR_STEP_SIZE = -984;

#endif
