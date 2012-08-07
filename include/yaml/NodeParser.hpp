#ifndef NODE_PARSER_HPP
#define NODE_PARSER_HPP

#include <yaml-cpp/yaml.h>

namespace yaml
{
	const YAML::Node& ParseNode( const YAML::Node& nodes, const std::string& module_name  )
	{
		std::string name;

		for( int i=0; i<nodes.size(); ++i)
		{
			const YAML::Node& node = nodes[i];
			node["module_name"] >> name;
			if( name == module_name)
			{
				return node;
			}
		}
	}
};

#endif

