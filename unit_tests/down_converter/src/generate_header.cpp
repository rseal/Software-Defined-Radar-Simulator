// Copyright (c) 2010 Ryan Seal <rlseal -at- gmail.com>
//
// This file is part of Software Defined Radar Simulator (SDRS) Software.
//
// SDRS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//  
// SDRS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SDRS.  If not, see <http://www.gnu.org/licenses/>.
#include <iostream>

#include <tr1/math.h>

#include <sdr_simulator/xml/DownConverterXmlParser.hpp>
#include <sdr_simulator/util/CodeGenerator.hpp>

#include "StimulusXmlParser.hpp"

using namespace std;
using namespace code_generator;
using namespace boost;

int sc_main(int argc, char* argv[])
{
   const std::string CONFIG_FILE_NAME = argv[1];
   const std::string STIMULUS_FILE_NAME = argv[2];
   const string HEADER_FILE_NAME = "test_bench.hpp";

   // open the configuration file for parsing
   ticpp::Document doc( CONFIG_FILE_NAME );

   // Create a parser object
   DownConverterXmlParser ddc_parser;

   // Parse the xml file.
   doc.LoadFile();

   // Use the root node for reference.
   ticpp::Node* root = doc.FirstChild();

   // find the first module node in the xml file
   ticpp::Node* node = root->FirstChildElement( ddc_parser.Name() );

   // Retrieve a map containing accumulator keywords
   xml::NodeMap config_map = ddc_parser.Parse( node ); 

   // create constants that were read in from the xml file.
   const int DATA_WIDTH = lexical_cast<int>( config_map["xy_width"] );
   const int PHASE_WIDTH = lexical_cast<double>( config_map["z_width"] );
   const int NUM_STAGES = lexical_cast<double>( config_map["num_stages"] );


   // Create a parser object
   StimulusXmlParser stimulus_parser;

   doc.LoadFile( STIMULUS_FILE_NAME );
   root = doc.FirstChildElement( stimulus_parser.Name() );
   xml::NodeMap stimulus_map = stimulus_parser.Parse( root );

   const double SIGNAL_FREQUENCY = 
      lexical_cast<double>( stimulus_map["signal_frequency"] );
   const double DDC_FREQUENCY = 
      lexical_cast<double>( stimulus_map["ddc_frequency"] );
   const double SAMPLE_RATE = 
      lexical_cast<double>( stimulus_map["sample_rate"] );

   const double NORMALIZED_FREQUENCY = SIGNAL_FREQUENCY/SAMPLE_RATE;
   const double SCALE = tr1::pow(2.0, PHASE_WIDTH);
   const int ACCUMULATOR_STEP_SIZE = rint( SCALE*DDC_FREQUENCY/SAMPLE_RATE );

   // create a CodeGenerator object. This is required to generate the
   // header file. 
   code_generator::CodeGenerator code_generator;

   // generate data input type
   code_generator.AddTypeDef(
         "INPUT_DATA_TYPE",
         "sc_int<" + lexical_cast< string >( DATA_WIDTH ) + ">"
         );

   // generate data output type
   code_generator.AddTypeDef(
         "OUTPUT_DATA_TYPE",
         "sc_int<" + lexical_cast< string >( DATA_WIDTH ) + ">"
         );

   // generate z data output type
   code_generator.AddTypeDef(
         "PHASE_DATA_TYPE",
         "sc_int<" + lexical_cast< string >( PHASE_WIDTH ) + ">"
         );

   code_generator.AddTypeDef(
         "RESET_TYPE",
         "bool"
         );

   //TODO: Remove and place in stimulus config
   code_generator.AddConstant<int>(
         "ADC_WIDTH",
         12
         );

   code_generator.AddConstant<int>(
         "DATA_WIDTH",
         DATA_WIDTH
         );
   
   code_generator.AddConstant<int>(
         "PHASE_WIDTH",
         PHASE_WIDTH
         );

   code_generator.AddConstant<int>(
         "NUM_STAGES",
         NUM_STAGES
         );

   code_generator.AddConstant<double>(
         "NORMALIZED_FREQUENCY",
         NORMALIZED_FREQUENCY
         );

   code_generator.AddConstant<double>(
         "DDC_FREQUENCY",
         DDC_FREQUENCY
         );

   code_generator.AddConstant<double>(
         "SAMPLE_RATE",
         SAMPLE_RATE); 

   code_generator.AddConstant<int>(
         "ACCUMULATOR_STEP_SIZE",
         ACCUMULATOR_STEP_SIZE
         ); 

   code_generator.AddInclude( "systemc.h", true );

   code_generator.GenerateFile( HEADER_FILE_NAME );

   return EXIT_SUCCESS;
}
