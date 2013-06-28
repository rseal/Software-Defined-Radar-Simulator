#include <iostream>
#include <sdr_simulator/yaml/ConfigNode.hpp>
#include <sdr_simulator/yaml/NodeFactory.hpp>

int main(int argc, char* argv[])
{
   
   try
   {
      YAML::Node config = YAML::LoadFile(argv[1]);
      YAML::const_iterator iter =config.begin();
      yaml::ConfigNodePtr np;

      while( iter != config.end())
      {
         const YAML::Node& node = *iter;

         std::string module_name = node["module_name"].as<std::string>();
         
         np = yaml::NodeFactory::Instance().Create( module_name );
         np->Decode(node);
         np->Print(std::cout);
         ++iter;
      }

   }
   catch ( YAML::ParserException& e )
   {
      std::cout << e.what();
   }

   return 0;
}

