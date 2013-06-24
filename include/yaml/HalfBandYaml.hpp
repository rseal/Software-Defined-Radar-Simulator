#ifndef HALFBAND_YAML_HPP
#define HALFBAND_YAML_HPP

#include <iostream>
#include <sdr_simulator/yaml/ConfigNode.hpp>

namespace yaml
{
   class HalfBandYaml : public ConfigNode
   {
      public:

      std::string moduleName;
		std::string model;
		int inputWidth;
		int outputWidth;

      HalfBandYaml(): ConfigNode("half_band_filter") {}

      YAML::Node Encode()
      {
			YAML::Node node;
			node["module_name"]  = moduleName;
			node["model"]        = model;
			node["input_width"]  = inputWidth;
			node["output_width"] = outputWidth;
			return node;
		}

		void Decode( const YAML::Node& node)
		{
			moduleName  = node["module_name"].as<std::string>();
			model       = node["model"].as<std::string>();
			inputWidth  = node["input_width"].as<int>();
			outputWidth = node["output_width"].as<int>();
		}

		void Print(std::ostream& os)
		{
			os << "name         : " << moduleName  << "\n"
				<< "model        : " << model       << "\n"
				<< "input_width  : " << inputWidth  << "\n"
				<< "output_width : " << outputWidth << "\n";
		}
	};
};

#endif
