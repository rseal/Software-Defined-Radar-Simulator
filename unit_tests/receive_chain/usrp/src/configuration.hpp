#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <systemc>
#include <list>
#include <tr1/math.h>

// top-level settings
namespace usrp
{
const unsigned int DATA_WIDTH= 16;
const unsigned int DECIMATION = 8;
typedef sc_int<DATA_WIDTH> INPUT_TYPE;
typedef sc_int<DATA_WIDTH> OUTPUT_TYPE;
typedef sc_int<DATA_WIDTH> DECIMATION_TYPE;
typedef bool RESET_TYPE;
}

namespace cic
{
   const unsigned int DATA_WIDTH= 16;
   const unsigned int PHASE_WIDTH = 16;
   const unsigned int NUM_STAGES = 5;
   typedef sc_int<DATA_WIDTH> INPUT_TYPE;
   typedef sc_int<DATA_WIDTH> OUTPUT_TYPE;
};

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
   const double TUNING_FREQUENCY = -14.2e6;
   typedef sc_int< DATA_WIDTH > DATA_TYPE;

   // static helper function
   static const sc_int< DATA_WIDTH> StepSize() { 
      return rint( std::tr1::pow(2.0,DATA_WIDTH) * TUNING_FREQUENCY / SAMPLE_RATE );
   }
};

namespace filter
{
   const unsigned int COEFF_WIDTH = 16;
   const unsigned int DATA_WIDTH = 16;
   const unsigned int SUM_WIDTH = 32;

   // filter input/output data types
   typedef sc_int<DATA_WIDTH> INPUT_TYPE;
   typedef sc_int<DATA_WIDTH> OUTPUT_TYPE;
   typedef sc_int<DATA_WIDTH> DECIMATION_TYPE;

   typedef std::list< sc_int< COEFF_WIDTH > > coeff_list;

   // NOTE: this notation is only compatible with the newer c++0x standard.
   const coeff_list
      COEFFICIENTS = { 
         -49,0,165,0,-412,0,873,0,-1681,0,3135,0,-6282,0,20628,32767,
         20628,0,-6282,0,3135,0,-1681,0,873,0,-412,0,165,0,-49
      };
}

// debug only 
namespace debug
{
   typedef sc_export< sc_signal<filter::OUTPUT_TYPE> > sc_export_cic_output;
   typedef sc_export< sc_signal<cordic::OUTPUT_TYPE> > sc_export_cordic_output;
   typedef sc_export< sc_signal_inout_if<bool> > sc_export_clk;
}

   
const int ADC_WIDTH = 12;
const double SIGNAL_FREQUENCY = 2.00e6;
const double SAMPLE_RATE = 64e6;
const double NORMALIZED_FREQUENCY = SIGNAL_FREQUENCY/SAMPLE_RATE;

#endif
