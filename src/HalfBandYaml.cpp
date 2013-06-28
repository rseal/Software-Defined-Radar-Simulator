#include<sdr_simulator/yaml/HalfBandYaml.hpp>
#include <sdr_simulator/yaml/NodeFactory.hpp>

namespace yaml {

	YAML::Node HalfBandYaml::Encode()
	{
		YAML::Node node;
		node["module_name"]  = moduleName;
		node["model"]        = model;
		node["input_width"]  = inputWidth;
		node["output_width"] = outputWidth;
		return node;
	}

	void HalfBandYaml::Decode( const YAML::Node& node)
	{
		moduleName  = node["module_name"].as<std::string>();
		model       = node["model"].as<std::string>();
		inputWidth  = node["input_width"].as<int>();
		outputWidth = node["output_width"].as<int>();
	}

	void HalfBandYaml::Print(std::ostream& os)
	{
		os << "name         : " << moduleName  << "\n"
			<< "model        : " << model       << "\n"
			<< "input_width  : " << inputWidth  << "\n"
			<< "output_width : " << outputWidth << "\n";
	}
}


namespace{
   yaml::ConfigNodePtr Callback() { return yaml::ConfigNodePtr( new yaml::HalfBandYaml() );}
   bool registered = yaml::NodeFactory::Instance().Register("half_band_filter",Callback);
}
