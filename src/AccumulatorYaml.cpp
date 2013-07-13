#include <sdr_simulator/yaml/AccumulatorYaml.hpp>
#include <sdr_simulator/yaml/NodeFactory.hpp>

namespace yaml{

	YAML::Node AccumulatorYaml::Encode()
	{
		YAML::Node node;
		node["module_name"]  = moduleName;
		node["input_width"]  = inputWidth;
		node["output_width"] = outputWidth;
		node["sample_rate"]  = sampleRate;
		return node;
	}

	void AccumulatorYaml::Decode( const YAML::Node& node)
	{
		inputWidth  = node["input_width"].as<int>();
		outputWidth = node["output_width"].as<int>();
		sampleRate  = node["sample_rate"].as<double>();
	}

	void AccumulatorYaml::Print(std::ostream& os)
	{
		os << "module_name    : " << moduleName  << "\n"
		   << "input_width    : " << inputWidth  << "\n"
		   << "output_width   : " << outputWidth << "\n"
		   << "sample_rate    : " << sampleRate  << "\n";
	}

}

namespace{
   yaml::ConfigNodePtr Callback() { return yaml::ConfigNodePtr( new yaml::AccumulatorYaml() );}
   bool registered = yaml::NodeFactory::Instance().Register("accumulator",Callback);
}

