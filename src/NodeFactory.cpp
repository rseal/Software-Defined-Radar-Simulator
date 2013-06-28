#include<sdr_simulator/yaml/NodeFactory.hpp>

namespace yaml
{
	NodeFactory* NodeFactory::pInstance_ = 0;
	bool NodeFactory::destroyed_ = false;
}
