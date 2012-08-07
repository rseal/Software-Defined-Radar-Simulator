#ifndef CORDIC_YAML_HPP
#define CORDIC_YAML_HPP 

#include <iostream>
#include <yaml-cpp/yaml.h>

namespace yaml
{
   struct CordicYaml
   {
      std::string moduleName;
      std::string model;
      int xyWidth;
      int zWidth;
      double sampleRate;
      double tuningFrequency;

      friend std::ostream& operator << ( std::ostream& os, const CordicYaml& obj)
      {
         os << "name             : " << obj.moduleName      << "\n"
            << "model            : " << obj.model           << "\n"
            << "xy_width         : " << obj.xyWidth         << "\n"
            << "z_width          : " << obj.zWidth          << "\n"
            << "samplerate       : " << obj.sampleRate      << "\n"
            << "tuning_frequency : " << obj.tuningFrequency << "\n";

         return os;
      }
   };

   void operator >> ( const YAML::Node& node, CordicYaml& obj )
   {
      node["module_name"]      >> obj.moduleName;
      node["model"]            >> obj.model;
      node["xy_width"]         >> obj.xyWidth;
      node["z_width"]          >> obj.zWidth;
      node["sample_rate"]      >> obj.sampleRate;
      node["tuning_frequency"] >> obj.tuningFrequency;
   }
};

#endif
