#ifndef CORDIC_YAML_HPP
#define CORDIC_YAML_HPP 

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <sdr_simulator/yaml/ConfigNode.hpp>

namespace yaml
{
   class CordicYaml;
   typedef boost::shared_ptr<CordicYaml> CordicYamlPtr;

   class CordicYaml : public ConfigNode
   {
      public:

      std::string model;
      int xyWidth;
      int zWidth;
      int numStages;
      double sampleRate;
      double tuningFrequency;

      CordicYaml(): ConfigNode("ddc") {}

		YAML::Node Encode();
		void Decode( const YAML::Node& node);
		void Print(std::ostream& os);

	};
};

#endif
