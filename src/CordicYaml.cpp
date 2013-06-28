#include<sdr_simulator/yaml/CordicYaml.hpp>
#include <sdr_simulator/yaml/NodeFactory.hpp>

namespace yaml{

   YAML::Node CordicYaml::Encode()
   {
      YAML::Node node;
      node["module_name"]      = moduleName;
      node["model"]            = model;
      node["xy_width"]         = xyWidth;
      node["z_width"]          = zWidth;
      node["sample_rate"]      = sampleRate;
      node["tuning_frequency"] = tuningFrequency;
      return node;
   }

   void CordicYaml::Decode( const YAML::Node& node)
   {
      model           = node["model"].as<std::string>();
      xyWidth         = node["xy_width"].as<int>();
      zWidth          = node["z_width"].as<int>();
      sampleRate      = node["sample_rate"].as<double>();
      tuningFrequency = node["tuning_frequency"].as<double>();
   }

   void CordicYaml::Print(std::ostream& os)
   {
      os << "module_name      : " << moduleName      << "\n"
         << "model            : " << model           << "\n"
         << "xy_width         : " << xyWidth         << "\n"
         << "z_width          : " << zWidth          << "\n"
         << "samplerate       : " << sampleRate      << "\n"
         << "tuning_frequency : " << tuningFrequency << "\n";
   }
}

namespace{
   yaml::ConfigNodePtr Callback() { return yaml::ConfigNodePtr( new yaml::CordicYaml() );}
   bool registered = yaml::NodeFactory::Instance().Register("down_converter",Callback);
}
