#ifndef HALFBAND_YAML_HPP
#define HALFBAND_YAML_HPP

#include <iostream>
#include <yaml-cpp/yaml.h>

namespace yaml
{
   struct HalfBandYaml
   {
      std::string moduleName;
		std::string model;
		int inputWidth;
		int outputWidth;

      friend std::ostream& operator << ( std::ostream& os, const HalfBandYaml& obj)
      {
         os << "name         : " << obj.moduleName  << "\n"
				<< "model        : " << obj.model       << "\n"
				<< "input_width  : " << obj.inputWidth  << "\n"
				<< "output_width : " << obj.outputWidth << "\n";

         return os;
      }

   };

   void operator >> ( const YAML::Node& node, HalfBandYaml& obj )
   {
      node["module_name"]  >> obj.moduleName;
      node["model"]        >> obj.model;
      node["input_width"]  >> obj.inputWidth;
      node["output_width"] >> obj.outputWidth;
   }


};

#endif
