#ifndef ADC_YAML_HPP
#define ADC_YAML_HPP

#include <iostream>
#include <sdr_simulator/yaml/ConfigNode.hpp>

namespace yaml
{
   class AdcYaml : public ConfigNode
   {
      public:

      std::string model;
      int bitWidth;
		double vfs;
		double snr;
		double bw;


      AdcYaml(): ConfigNode("adc") {}

      YAML::Node Encode()
      {
         YAML::Node node;
         node["module_name"] = moduleName;
         node["model"]       = model;
         node["snr"]         = snr;
         node["vfs"]         = vfs;
         node["bw"]          = bw;
         node["bit_width"]   = bitWidth;
         return node;
      }

      void Decode( const YAML::Node& node)
      {
         model      = node["model"].as<std::string>();
         bitWidth   = node["bit_width"].as<int>();
         snr        = node["snr"].as<double>();
         vfs        = node["vfs"].as<double>();
         bw         = node["bw"].as<double>();
      }

      void Print(std::ostream& os)
      {
         os << "module_name    : " << moduleName << "\n"
            << "model          : " << model      << "\n"
            << "bitwidth       : " << bitWidth   << "\n"
            << "snr            : " << snr        << "\n"
            << "vfs            : " << vfs        << "\n"
            << "bw             : " << bw         << "\n";
      }

   };
};

#endif
