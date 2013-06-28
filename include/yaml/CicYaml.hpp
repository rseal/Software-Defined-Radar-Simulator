#ifndef CIC_YAML_HPP
#define CIC_YAML_HPP

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <sdr_simulator/yaml/ConfigNode.hpp>

namespace yaml
{
   class CicYaml;
   typedef boost::shared_ptr<CicYaml> CicYamlPtr;

   class CicYaml : public ConfigNode
   {
      public:

		int inputWidth;
		int outputWidth;
		int minDecimation;
		int maxDecimation;
		int differentialDelay;
		int numStages;
		bool useBitPruning;

      CicYaml(): ConfigNode("cic_filter") {}

		YAML::Node Encode();
		void Decode( const YAML::Node& node);
		void Print(std::ostream& os);

	};
};

#endif
