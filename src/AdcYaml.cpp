#include <sdr_simulator/yaml/AdcYaml.hpp>
#include <sdr_simulator/yaml/NodeFactory.hpp>

namespace yaml
{
	YAML::Node AdcYaml::Encode()
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

	void AdcYaml::Decode( const YAML::Node& node)
	{
		model      = node["model"].as<std::string>();
		bitWidth   = node["bit_width"].as<int>();
		snr        = node["snr"].as<double>();
		vfs        = node["vfs"].as<double>();
		bw         = node["bw"].as<double>();
	}

	void AdcYaml::Print(std::ostream& os)
	{
		os << "module_name    : " << moduleName << "\n"
			<< "model          : " << model      << "\n"
			<< "bitwidth       : " << bitWidth   << "\n"
			<< "snr            : " << snr        << "\n"
			<< "vfs            : " << vfs        << "\n"
			<< "bw             : " << bw         << "\n";
	}
};

namespace{
   yaml::ConfigNodePtr Callback() { return yaml::ConfigNodePtr( new yaml::AdcYaml() );}
   bool registered = yaml::NodeFactory::Instance().Register("adc",Callback);
}
