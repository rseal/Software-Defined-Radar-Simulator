#ifndef ACCUMULATOR_YAML_HPP
#define ACCUMULATOR_YAML_HPP

#include <iostream>
#include <yaml-cpp/yaml.h>

namespace yaml
{
   struct AccumulatorYaml
   {
      std::string moduleName;
      int bitWidth;
      double sampleRate;

      friend std::ostream& operator << ( std::ostream& os, const AccumulatorYaml& obj)
      {
         os << "name             : " << obj.moduleName      << "\n"
            << "bitwidth         : " << obj.bitWidth        << "\n"
            << "samplerate       : " << obj.sampleRate      << "\n";

         return os;
      }

   };

   void operator >> ( const YAML::Node& node, AccumulatorYaml& obj )
   {
      node["module_name"]      >> obj.moduleName;
      node["bit_width"]        >> obj.bitWidth;
      node["sample_rate"]      >> obj.sampleRate;
   }


};

#endif
