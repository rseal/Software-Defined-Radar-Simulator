#ifndef SIGNAL_GENERATOR_HPP
#define SIGNAL_GENERATOR_HPP

#include<systemc.h>
#include<vector>

template< unsigned int BIT_WIDTH >
class SignalGenerator : public sc_module
{
   typedef sc_uint<1> bit_type;
   typedef sc_int< BIT_WIDTH > sample_type;

   void Compute()
   {
      static int index = 0;

      if( index == samples_.size() || reset.read() == 1 ) index = 0;

      std::cout << "index = " << index << std::endl;

      output = samples_[index++];
   }

   // provide definition in derived class
   virtual void GenerateSamples() =0;

   protected:

   const int SAMPLE_SIZE;
   std::vector< sample_type > samples_;

   public:

      SC_HAS_PROCESS( SignalGenerator );

      SignalGenerator( const sc_module_name& nm, const int sampleSize):
         sc_module( nm ), SAMPLE_SIZE( sampleSize ), samples_(sampleSize) {
            
            SC_METHOD( Compute );
            sensitive << clock.pos();

         }

      sc_in< bit_type > reset;
      sc_in_clk clock;
      sc_out< sample_type > output;

      // must call this prior to use
      void Init(){
         GenerateSamples();
      }
};

#endif
