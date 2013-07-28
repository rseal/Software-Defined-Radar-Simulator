#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <systemc>
#include <boost/shared_ptr.hpp>
#include "Cic.hpp"

namespace sdr
{
typedef bool RESET_TYPE;
};

namespace filter
{
   const int INPUT_WIDTH = 16;
   const int OUTPUT_WIDTH = 16;
   const int DECIMATION_WIDTH = 16;

   typedef sc_int<INPUT_WIDTH> INPUT_TYPE;
   typedef sc_int<OUTPUT_WIDTH> OUTPUT_TYPE;
   typedef sc_int<DECIMATION_WIDTH> DECIMATION_TYPE;

   // cic definitions
   typedef Cic<INPUT_TYPE,OUTPUT_TYPE> CicFilter;
   typedef boost::shared_ptr<CicFilter> CicPtr;
};

namespace accumulator{

   const int ACC_WIDTH    = 32;
   const int OUTPUT_WIDTH = 16;

   typedef sc_int<ACC_WIDTH> ACC_TYPE;
   typedef sc_int<OUTPUT_WIDTH> OUTPUT_TYPE;

   typedef PhaseAccumulator<ACC_TYPE,OUTPUT_TYPE> PhaseAccum16Bit;
   typedef boost::shared_ptr< PhaseAccum16Bit > AccumulatorPtr;
};

namespace down_converter{

   const int NUM_STAGES   = 12;
   const int INPUT_WIDTH  = 16;
   const int OUTPUT_WIDTH = 16;
   const int PHASE_WIDTH  = 16;

   typedef sc_int<INPUT_WIDTH> INPUT_TYPE;
   typedef sc_int<OUTPUT_WIDTH> OUTPUT_TYPE;
   typedef sc_int<PHASE_WIDTH> PHASE_TYPE;

   typedef Cordic<INPUT_TYPE,
           OUTPUT_TYPE,
           PHASE_TYPE,
           sdr::RESET_TYPE,
           INPUT_WIDTH,
           PHASE_WIDTH,
           NUM_STAGES> Cordic12Stage;

   typedef boost::shared_ptr< Cordic12Stage> CordicPtr;
}

namespace usrp_v1
{
   const int INPUT_WIDTH = 16;
   const int OUTPUT_WIDTH = 16;

   typedef sc_int<INPUT_WIDTH> INPUT_TYPE;
   typedef sc_int<OUTPUT_WIDTH> OUTPUT_TYPE;
};

#endif
