#ifndef CORDIC_YAML_HPP
#define CORDIC_YAML_HPP 

#include <iostream>
#include <sdr_simulator/yaml/ConfigNode.hpp>

namespace yaml
{
   class CordicYaml : public ConfigNode
   {
      public:

      std::string model;
      int xyWidth;
      int zWidth;
      double sampleRate;
      double tuningFrequency;

      CordicYaml(): ConfigNode("down_converter") {}

      YAML::Node Encode()
      {
         YAML::Node node;
			node["module_name"]      = moduleName;
			node["model"]            = model;
			node["xy_width"]         = xyWidth;
			node["z_width"]          = zWidth;
			node["sample_rate"]      = sampleRate;
			node["tuning_frequency"] = tuningFrequency;
         return node;
      }

      void Decode( const YAML::Node& node)
      {
         model           = node["model"].as<std::string>();
         xyWidth         = node["xy_width"].as<int>();
         zWidth          = node["z_width"].as<int>();
         sampleRate      = node["sample_rate"].as<double>();
         tuningFrequency = node["tuning_frequency"].as<double>();
      }

      void Print(std::ostream& os)
      {
         os << "module_name      : " << moduleName      << "\n"
            << "model            : " << model           << "\n"
            << "xy_width         : " << xyWidth         << "\n"
            << "z_width          : " << zWidth          << "\n"
            << "samplerate       : " << sampleRate      << "\n"
            << "tuning_frequency : " << tuningFrequency << "\n";
      }
	};
};

#endif
