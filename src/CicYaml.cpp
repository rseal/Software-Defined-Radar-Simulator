#include<sdr_simulator/yaml/CicYaml.hpp>
#include<sdr_simulator/yaml/NodeFactory.hpp>

namespace yaml{

	YAML::Node CicYaml::Encode()
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

	void CicYaml::Decode( const YAML::Node& node)
	{
		inputWidth        = node["input_width"].as<int>();
		outputWidth       = node["output_width"].as<int>();
		minDecimation     = node["min_decimation"].as<int>();
		maxDecimation     = node["max_decimation"].as<int>();
		differentialDelay = node["differential_delay"].as<int>();
		numStages         = node["num_stages"].as<int>();
		useBitPruning     = node["use_bit_pruning"].as<bool>();
	}

	void CicYaml::Print(std::ostream& os)
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
}

namespace{
   yaml::ConfigNodePtr Callback() { return yaml::ConfigNodePtr( new yaml::CicYaml() );}
   bool registered = yaml::NodeFactory::Instance().Register("cic_filter",Callback);
}
