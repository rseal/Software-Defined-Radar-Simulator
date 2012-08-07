#ifndef ADC_YAML_HPP
#define ADC_YAML_HPP

#include <iostream>
#include <yaml-cpp/yaml.h>

namespace yaml
{
   struct AdcYaml
   {
      std::string moduleName;
      std::string model;
      int bitWidth;

      friend std::ostream& operator << ( std::ostream& os, const AdcYaml& obj)
      {
         os << "name      : " << obj.moduleName << "\n"
            << "model     : " << obj.model      << "\n"
            << "bit_width : " << obj.bitWidth   << "\n";

         return os;
      }

   };

   void operator >> ( const YAML::Node& node, AdcYaml& obj )
   {
      node["module_name"] >> obj.moduleName;
      node["model"]       >> obj.model;
      node["bit_width"]   >> obj.bitWidth;
   }


};

#endif
