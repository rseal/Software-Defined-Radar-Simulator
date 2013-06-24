#ifndef ACCUMULATOR_YAML_HPP
#define ACCUMULATOR_YAML_HPP

#include <iostream>
#include <sdr_simulator/yaml/ConfigNode.hpp>

namespace yaml
{
   class AccumulatorYaml : public ConfigNode
   {
      public:

      int bitWidth;
		double sampleRate;

      AccumulatorYaml(): ConfigNode("accumulator") {}

      YAML::Node Encode()
      {
         YAML::Node node;
			node["module_name"] = moduleName;
         node["bit_width"]   = bitWidth;
         node["sample_rate"] = sampleRate;
         return node;
      }

      void Decode( const YAML::Node& node)
      {
         bitWidth   = node["bit_width"].as<int>();
         sampleRate = node["sample_rate"].as<double>();
      }

      void Print(std::ostream& os)
      {
         os << "module_name    : " << moduleName << "\n"
            << "bit_width      : " << bitWidth   << "\n"
            << "sample_rate    : " << sampleRate << "\n";
      }
	};
};

#endif
