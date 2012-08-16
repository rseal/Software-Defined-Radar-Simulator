#ifndef PULSE_GENERATOR_YAML_HPP
#define PULSE_GENERATOR_YAML_HPP

#include <iostream>
#include <yaml-cpp/yaml.h>

namespace yaml
{
	struct PulseGeneratorYaml
	{
		std::string moduleName;
		double pulseWidth;
		double pri;
		double rfFrequency;
		double doppler;
		double voltage;

		// TODO: Implement phase coding.
		std::string codeType;
		std::string codeName;
		int codeLength;
		std::vector<int> code;

		friend std::ostream& operator << ( std::ostream& os, const PulseGeneratorYaml& obj)
		{
			os 
				<< "module_name  : " << obj.moduleName  << "\n"
				<< "pulse_width  : " << obj.pulseWidth  << "\n"
				<< "pri          : " << obj.pri         << "\n"
				<< "voltage      : " << obj.voltage     << "\n"
				<< "code_type    : " << obj.codeType    << "\n"
				<< "code_name    : " << obj.codeName    << "\n"
				<< "rf_frequency : " << obj.rfFrequency << "\n"
				<< "doppler      : " << obj.doppler     << "\n";

			return os;
		}

	};

	void operator >> ( const YAML::Node& node, PulseGeneratorYaml& obj )
	{
		node["module_name"]  >> obj.moduleName;
		node["pulse_width"]  >> obj.pulseWidth;
		node["pri"]          >> obj.pri;
		node["voltage"]      >> obj.voltage;
		node["code_type"]    >> obj.codeType;
		node["code_name"]    >> obj.codeName;
		node["rf_frequency"] >> obj.rfFrequency;
		node["doppler"]      >> obj.doppler;
	}

};

#endif

