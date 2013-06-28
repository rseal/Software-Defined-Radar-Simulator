#ifndef CONFIG_NODE
#define CONFIG_NODE

#include <yaml-cpp/yaml.h>


namespace yaml
{
   class ConfigNode;
   typedef boost::shared_ptr<yaml::ConfigNode> ConfigNodePtr;

	class ConfigNode
	{
		protected:

			std::string moduleName;

		public:

			ConfigNode( const std::string& moduleName) : moduleName(moduleName){};

			virtual YAML::Node Encode()=0;
			virtual void Decode( const YAML::Node& node)=0;
			virtual void Print( std::ostream& os)=0;
	};
}

#endif
