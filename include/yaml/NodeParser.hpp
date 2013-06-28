#ifndef NODE_PARSER_HPP
#define NODE_PARSER_HPP

#include <iostream>

#include <boost/shared_ptr.hpp>

#include <yaml-cpp/yaml.h>
#include <sdr_simulator/yaml/NodeFactory.hpp>

namespace yaml
{
   struct NodeParser
   {
      template<class T>
      static boost::shared_ptr<T> ParseNode(const YAML::Node& nodes, const std::string& moduleName)
      {
         bool found=false;
         ConfigNodePtr node_ptr;

         YAML::const_iterator it = nodes.begin();

         while( it != nodes.end())
         {
            const YAML::Node& node = *it;
            std::string module_name = node["module_name"].as<std::string>();

            if( module_name == moduleName )
            {
               node_ptr = NodeFactory::Instance().Create(moduleName);
               node_ptr->Decode(node);
               node_ptr->Print(std::cout);
               found=true;
               break;
            }

            ++it;
         }

         if(!found) std::cerr << "ERROR: Unable to find node " << moduleName << std::endl;

         return boost::static_pointer_cast<T>(node_ptr);
      }
   };
};

#endif

