#ifndef TEST_SIGNAL_GENERATOR_HPP
#define TEST_SIGNAL_GENERATOR_HPP

#include<boost/random/linear_congruential.hpp>
#include<boost/random/normal_distribution.hpp>
#include<boost/random/variate_generator.hpp>

#include<sdr_simulator/SignalGenerator.hpp>
#include<iostream>
#include<tr1/math.h>

template< unsigned int BIT_WIDTH>
class TestSignalGenerator: public SignalGenerator<BIT_WIDTH>
{
   const int SCALE;
   typedef SignalGenerator<BIT_WIDTH> SigGen;

   virtual void GenerateSamples() {

      // random number number generating algorithm 
      boost::rand48 rng;
      // gaussian distribution
      boost::normal_distribution<> nDistribution(0,1);
      // create the random number generator
      boost::variate_generator< boost::rand48&, boost::normal_distribution<> > random_number(rng,nDistribution);

      for( int i=0; i< this->SAMPLE_SIZE; ++i) {
         this->samples_[i] = random_number()*SCALE;
      }

   }
   
   public:

   SC_HAS_PROCESS( TestSignalGenerator );

   TestSignalGenerator( const sc_module_name& nm, const int sampleSize ):
      SignalGenerator<BIT_WIDTH>( nm, sampleSize ), 
      SCALE( std::tr1::pow(2.0,BIT_WIDTH)-1 )
   {
      this->Init();
   }

};


#endif
