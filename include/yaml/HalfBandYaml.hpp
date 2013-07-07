#ifndef HALFBAND_YAML_HPP
#define HALFBAND_YAML_HPP

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <sdr_simulator/yaml/ConfigNode.hpp>

namespace yaml
{
   class HalfBandYaml;
   typedef boost::shared_ptr<HalfBandYaml> HalfBandYamlPtr;

   class HalfBandYaml : public ConfigNode
   {
      public:

      std::string moduleName;
		std::string model;
		int inputWidth;
		int outputWidth;
		int coeffWidth;

      HalfBandYaml(): ConfigNode("half_band_filter") {}

		YAML::Node Encode();
		void Decode( const YAML::Node& node);
		void Print(std::ostream& os);

	};
};

#endif
