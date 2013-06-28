#ifndef ACCUMULATOR_YAML_HPP
#define ACCUMULATOR_YAML_HPP

#include <iostream>
#include <sdr_simulator/yaml/ConfigNode.hpp>

namespace yaml
{
   class AccumulatorYaml;
   typedef boost::shared_ptr<AccumulatorYaml> AccumulatorYamlPtr;

   class AccumulatorYaml : public ConfigNode
   {
      public:

      int bitWidth;
		double sampleRate;

      AccumulatorYaml(): ConfigNode("accumulator") {}

		YAML::Node Encode();
		void Decode( const YAML::Node& node);
		void Print(std::ostream& os);

	};
};

#endif
