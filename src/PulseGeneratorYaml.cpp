#include <sdr_simulator/yaml/PulseGeneratorYaml.hpp>
#include <sdr_simulator/yaml/NodeFactory.hpp>

namespace yaml
{

   YAML::Node PulseGeneratorYaml::Encode()
   {
      YAML::Node node;
      node["module_name"]  = moduleName;
      node["pulse_width"]  = pulseWidth;
      node["pri"]          = pri;
      node["voltage"]      = voltage;
      node["rf_frequency"] = rfFrequency;
      node["code_type"]    = codeType;
      node["code_name"]    = codeName;

      return node;
   }

   void PulseGeneratorYaml::Decode( const YAML::Node& node)
   {
      pulseWidth  = node["pulse_width"].as<double>();
      pri         = node["pri"].as<double>();
      voltage     = node["voltage"].as<double>();
      rfFrequency = node["rf_frequency"].as<double>();
      codeType    = node["code_type"].as<double>();
      codeName    = node["code_name"].as<double>();
   }

   void PulseGeneratorYaml::Print(std::ostream& os)
   {
      os
         << "module_name  : " << moduleName  << "\n"
         << "pulse_width  : " << pulseWidth  << "\n"
         << "pri          : " << pri         << "\n"
         << "voltage      : " << voltage     << "\n"
         << "code_type    : " << codeType    << "\n"
         << "code_name    : " << codeName    << "\n"
         << "rf_frequency : " << rfFrequency << "\n"
         << "doppler      : " << doppler     << "\n";
   }

}

namespace{
   yaml::ConfigNodePtr Callback() { return yaml::ConfigNodePtr( new yaml::PulseGeneratorYaml() );}
   bool registered = yaml::NodeFactory::Instance().Register("pulse_generator",Callback);
}

