#ifndef PULSE_GENERATOR_YAML_HPP
#define PULSE_GENERATOR_YAML_HPP

#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <sdr_simulator/yaml/ConfigNode.hpp>

namespace yaml
{
   class PulseGeneratorYaml;
   typedef boost::shared_ptr<PulseGeneratorYaml> PulseGeneratorYamlPtr;

   class PulseGeneratorYaml : public ConfigNode
   {
      public:

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

         PulseGeneratorYaml(): ConfigNode("pulse_generator") {}

         YAML::Node Encode();
         void Decode( const YAML::Node& node);
         void Print(std::ostream& os);

   };
};

#endif

