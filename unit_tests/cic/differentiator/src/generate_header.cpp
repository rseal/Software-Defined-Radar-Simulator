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

#include <sdr_simulator/filter/cic/CicGenerator.hpp>
#include <sdr_simulator/xml/CicXmlParser.hpp>
#include <sdr_simulator/util/CodeGenerator.hpp>

#include "StimulusXmlParser.hpp"

using namespace std;
using namespace code_generator;
using namespace boost;

int main(int argc, char* argv[])
{
   const std::string STIMULUS_FILE_NAME = argv[1];
   const string HEADER_FILE_NAME = "test_bench.hpp";

   // open the configuration file for parsing
   ticpp::Document doc( STIMULUS_FILE_NAME );

   // Create a parser object
   StimulusXmlParser stimulus_parser;

   // Parse the xml file.
   doc.LoadFile();

   // find the first module node in the xml file
   ticpp::Node* node = doc.FirstChildElement( stimulus_parser.Name() );

   // Retrieve a map containing accumulator keywords
   xml::NodeMap config_map = stimulus_parser.Parse( node ); 

   // create constants that were read in from the xml file.
   const int INPUT_WIDTH = lexical_cast<int>( config_map["input_width"] );
   const int OUTPUT_WIDTH = lexical_cast<int>( config_map["output_width"] );

   // create a CodeGenerator object. This is required to generate the
   // header file. 
   code_generator::CodeGenerator code_generator;

   // generate data input type
   code_generator.AddTypeDef(
         "INPUT_TYPE",
         "sc_int<" + lexical_cast< string >( INPUT_WIDTH ) + ">"
         );

   // generate data output type
   code_generator.AddTypeDef(
         "OUTPUT_TYPE",
         "sc_int<" + lexical_cast< string >( OUTPUT_WIDTH ) + ">"
         );

   code_generator.AddTypeDef(
         "RESET_TYPE",
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

   return EXIT_SUCCESS;
}
