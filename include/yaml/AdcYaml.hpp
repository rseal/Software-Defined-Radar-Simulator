#ifndef ADC_YAML_HPP
#define ADC_YAML_HPP

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <sdr_simulator/yaml/ConfigNode.hpp>

namespace yaml
{
   class AdcYaml;
   typedef boost::shared_ptr<AdcYaml> AdcYamlPtr;

   class AdcYaml : public ConfigNode
   {
      public:

      std::string model;
      int bitWidth;
		double vfs;
		double snr;
		double bw;

      AdcYaml(): ConfigNode("adc") {};

      YAML::Node Encode();
	   void Decode( const YAML::Node& node);
      void Print(std::ostream& os);

   };

};

#endif
