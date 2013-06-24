#include <sdr_simulator/yaml/AdcYaml.hpp>
#include <sdr_simulator/yaml/AccumulatorYaml.hpp>
#include <sdr_simulator/yaml/CordicYaml.hpp>
#include <sdr_simulator/yaml/CicYaml.hpp>
#include <sdr_simulator/yaml/HalfBandYaml.hpp>
//#include <sdr_simulator/yaml/NodeParser.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <sdr_simulator/yaml/ConfigNode.hpp>

int main(int argc, char* argv[])
{
   typedef boost::shared_ptr<yaml::ConfigNode> ConfigNodePtr;
   typedef std::map<std::string,ConfigNodePtr> ConfigNodeMap;
   ConfigNodeMap node_map;
   node_map["adc"] = ConfigNodePtr(new yaml::AdcYaml());
   node_map["accumulator"] = ConfigNodePtr(new yaml::AccumulatorYaml());
   node_map["down_converter"] = ConfigNodePtr(new yaml::CordicYaml());
   node_map["cic_filter"] = ConfigNodePtr(new yaml::CicYaml());
   node_map["half_band_filter"] = ConfigNodePtr(new yaml::HalfBandYaml());

   //yaml::CicYaml cic_data;
   //yaml::HalfBandYaml hb_data;

   //const YAML::Node& accumulator_node = yaml::ParseNode( config, "accumulator");
   //const YAML::Node& cordic_node = yaml::ParseNode( config, "down_converter");
   //const YAML::Node& cic_node = yaml::ParseNode( config, "cic");
   //const YAML::Node& filter_node = yaml::ParseNode( config, "filter");

   try
   {
      YAML::Node config = YAML::LoadFile(argv[1]);
      YAML::const_iterator iter =config.begin();
      ConfigNodePtr np;

      while( iter != config.end())
      {
         const YAML::Node& node = *iter;

         std::string module_name = node["module_name"].as<std::string>();
         
         ConfigNodeMap::iterator cn_iter = node_map.find(module_name);

         if( cn_iter != node_map.end())
         {
            np = node_map[module_name];
            np->Decode(node);
            np->Print(std::cout);
            YAML::Node new_node = np->Encode();
         }


         //np->Print(std::cout);
         //std::cout << node["module_name"] << std::endl;
         //if(node["module_name"]=="adc")
         //{
            //std::cout << "found adc node" << std::endl;
         //}
         //std::cout << node << std::endl;
         ++iter;
      }

      //yaml::AdcYaml::Decode( config, adc_data);
      //std::cout << adc_data;
      //adc_data = yaml::AdcYaml::Encode(adc_node);
		//adc_node >> adc_data;
      //std::cout << adc_data;

      //std::cout << std::endl;

      //accumulator_node >> acc_data;
      //std::cout << acc_data;

      //std::cout << std::endl;

      //cordic_node >> cordic_data;
      //std::cout << cordic_data;

      //std::cout << std::endl;

      //cic_node >> cic_data;
      //std::cout << cic_data;

      //std::cout << std::endl;

		//filter_node >> hb_data;
      //std::cout << hb_data;
   }
   catch ( YAML::ParserException& e )
   {
      std::cout << e.what();
   }

   return 0;
}

