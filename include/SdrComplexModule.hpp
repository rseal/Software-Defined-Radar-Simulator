#ifndef SDR_COMPLEX_MODULE_HPP
#define SDR_COMPLEX_MODULE_HPP

#include <systemc.h>

namespace sdr_module{

   typedef bool reset_type;

   template < typename DATA_INPUT_TYPE, typename DATA_OUTPUT_TYPE >
      class ComplexModule : public sc_module
   {
      // Implementation goes here.
      virtual void Compute() = 0;

      public:
         
      //systemc-specific macro
      SC_HAS_PROCESS( ComplexModule );

      ComplexModule( const sc_module_name& name ): sc_module(name)
      {
         // systemc-specific macro.
         SC_METHOD( Compute );
         sensitive << clock.pos();
      }

      sc_in_clk clock;
      sc_in< reset_type > reset;
      sc_in< DATA_INPUT_TYPE > real_input;
      sc_in< DATA_INPUT_TYPE > imag_input;
      sc_out< DATA_OUTPUT_TYPE > real_output;
      sc_out< DATA_OUTPUT_TYPE > imag_output;
   };
};

#endif 
