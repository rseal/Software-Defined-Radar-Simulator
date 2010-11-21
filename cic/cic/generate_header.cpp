#include <iostream>
#include <tr1/math.h>

#include <sdr_simulator/CicGenerator.hpp>
#include <sdr_simulator/xml/CicXmlParser.hpp>
#include <sdr_simulator/util/CodeGenerator.hpp>

#include "StimulusXmlParser.hpp"

using namespace std;
using namespace code_generator;
using namespace boost;

int main(void)
{
   const std::string CONFIGURATION_FILE_NAME = "../../config/sdr_config.xml";
   const std::string STIMULUS_FILE_NAME = "stimulus.xml";
   const string HEADER_FILE_NAME = "test_bench.hpp";

   // open the configuration file for parsing
   ticpp::Document doc( CONFIGURATION_FILE_NAME );

   // Create a parser object
   CicXmlParser cic_parser;

   // Parse the xml file.
   doc.LoadFile();

   // Use the root node for reference.
   ticpp::Node* root = doc.FirstChild();

   // find the first module node in the xml file
   ticpp::Node* node = root->FirstChildElement( cic_parser.Name() );

   // Retrieve a map containing accumulator keywords
   xml::NodeMap config_map = cic_parser.Parse( node ); 

   // create constants that were read in from the xml file.
   const int INPUT_WIDTH = lexical_cast<int>( config_map["input_width"] );
   const int OUTPUT_WIDTH = lexical_cast<int>( config_map["output_width"] );

   // create a CodeGenerator object. This is required to generate the
   // header file. 
   code_generator::CodeGenerator code_generator;

   // generate data input type
   code_generator.AddTypeDef(
         "data_input_type",
         "sc_int<" + lexical_cast< string >( INPUT_WIDTH ) + ">"
         );

   // generate data output type
   code_generator.AddTypeDef(
         "data_output_type",
         "sc_int<" + lexical_cast< string >( OUTPUT_WIDTH ) + ">"
         );

   code_generator.AddTypeDef(
         "reset_type",
         "bool"
         );

   code_generator.AddConstant<int>(
         "INPUT_WIDTH",
         INPUT_WIDTH
         );

   code_generator.AddConstant<int>(
         "OUTPUT_WIDTH",
         OUTPUT_WIDTH
         );

   code_generator.AddInclude( "systemc.h", true );

   code_generator.GenerateFile( HEADER_FILE_NAME );

   CicGenerator cicGenerator;

   return EXIT_SUCCESS;
}
