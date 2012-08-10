#ifndef CIC_YAML_HPP
#define CIC_YAML_HPP

#include <iostream>
#include <yaml-cpp/yaml.h>

namespace yaml
{
	struct CicYaml
	{
		std::string moduleName;
		int inputWidth;
		int outputWidth;
		int minDecimation;
		int maxDecimation;
		int differentialDelay;
		int numStages;
		bool useBitPruning;

		friend std::ostream& operator << ( std::ostream& os, const CicYaml& obj)
		{
			os  << "name               : " << obj.moduleName        << "\n"
				<< "input_width        : " << obj.inputWidth        << "\n"
				<< "output_width       : " << obj.outputWidth       << "\n"
				<< "min_decimation     : " << obj.minDecimation     << "\n"
				<< "max_decimation     : " << obj.maxDecimation     << "\n"
				<< "differential_delay : " << obj.differentialDelay << "\n"
				<< "num_stages         : " << obj.numStages         << "\n"
				<< "use_bit_pruning    : " << obj.useBitPruning     << "\n";

			return os;
		}
	};

	void operator >> ( const YAML::Node& node, CicYaml& obj )
	{
		node["module_name"]        >> obj.moduleName;
		node["input_width"]        >> obj.inputWidth;
		node["output_width"]       >> obj.outputWidth;
		node["min_decimation"]     >> obj.minDecimation;
		node["max_decimation"]     >> obj.maxDecimation;
		node["differential_delay"] >> obj.differentialDelay;
		node["num_stages"]         >> obj.numStages;
		node["use_bit_pruning"]    >> obj.useBitPruning;
	}
};

#endif
