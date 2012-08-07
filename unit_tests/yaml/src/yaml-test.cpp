#include <sdr_simulator/yaml/AccumulatorYaml.hpp>
#include <sdr_simulator/yaml/AdcYaml.hpp>
#include <sdr_simulator/yaml/CicYaml.hpp>
#include <sdr_simulator/yaml/CordicYaml.hpp>
#include <sdr_simulator/yaml/HalfBandYaml.hpp>
#include <sdr_simulator/yaml/NodeParser.hpp>

#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
   yaml::AdcYaml adc_data;
   yaml::AccumulatorYaml acc_data;
   yaml::CordicYaml cordic_data;
   yaml::CicYaml cic_data;
   yaml::HalfBandYaml hb_data;

   std::ifstream fin(argv[1]);
   YAML::Parser parser(fin);
   YAML::Node doc;
   parser.GetNextDocument(doc);

   const YAML::Node& adc_node = yaml::ParseNode( doc, "adc");
   const YAML::Node& accumulator_node = yaml::ParseNode( doc, "accumulator");
   const YAML::Node& cordic_node = yaml::ParseNode( doc, "down_converter");
   const YAML::Node& cic_node = yaml::ParseNode( doc, "cic");
   const YAML::Node& filter_node = yaml::ParseNode( doc, "filter");

   try
   {
		adc_node >> adc_data;
      std::cout << adc_data;

      std::cout << std::endl;

      accumulator_node >> acc_data;
      std::cout << acc_data;

      std::cout << std::endl;

      cordic_node >> cordic_data;
      std::cout << cordic_data;

      std::cout << std::endl;

      cic_node >> cic_data;
      std::cout << cic_data;

      std::cout << std::endl;

		filter_node >> hb_data;
      std::cout << hb_data;
   }
   catch ( YAML::ParserException& e )
   {
      std::cout << e.what();
   }

   return 0;
}

