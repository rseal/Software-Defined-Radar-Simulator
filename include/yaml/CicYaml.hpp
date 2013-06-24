#ifndef CIC_YAML_HPP
#define CIC_YAML_HPP

#include <iostream>
#include <sdr_simulator/yaml/ConfigNode.hpp>

namespace yaml
{
   class CicYaml : public ConfigNode
   {
      public:

		int inputWidth;
		int outputWidth;
		int minDecimation;
		int maxDecimation;
		int differentialDelay;
		int numStages;
		bool useBitPruning;

      CicYaml(): ConfigNode("cic_filter") {}

      YAML::Node Encode()
      {
			YAML::Node node;
			node["module_name"]        = moduleName;
			node["input_width"]        = inputWidth;
			node["output_width"]       = outputWidth;
			node["min_decimation"]     = minDecimation;
			node["max_decimation"]     = maxDecimation;
			node["differential_delay"] = differentialDelay;
			node["num_stages"]         = numStages;
			node["use_bit_pruning"]    = useBitPruning;
			return node;
		}

		void Decode( const YAML::Node& node)
		{
			inputWidth        = node["input_width"].as<int>();
			outputWidth       = node["output_width"].as<int>();
			minDecimation     = node["min_decimation"].as<int>();
			maxDecimation     = node["max_decimation"].as<int>();
			differentialDelay = node["differential_delay"].as<int>();
			numStages         = node["num_stages"].as<int>();
			useBitPruning     = node["use_bit_pruning"].as<bool>();
		}

		void Print(std::ostream& os)
		{
			os << "module_name        : " << moduleName        << "\n"
				<< "input_width        : " << inputWidth        << "\n"
				<< "output_width       : " << outputWidth       << "\n"
				<< "min_decimation     : " << minDecimation     << "\n"
				<< "max_decimation     : " << maxDecimation     << "\n"
				<< "differential_delay : " << differentialDelay << "\n"
				<< "num_stages         : " << numStages         << "\n"
				<< "use_bit_pruning    : " << useBitPruning     << "\n";
		}
	};
};

#endif
