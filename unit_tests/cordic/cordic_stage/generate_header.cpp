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

int main(void)
{
   const std::string CONFIG_FILE_NAME = "../../../config/sdr_config.xml";
   const std::string STIMULUS_FILE_NAME = "stimulus.xml";
   const string HEADER_FILE_NAME = "test_bench.hpp";

   // open the configuration file for parsing
   ticpp::Document doc( STIMULUS_FILE_NAME );

   // Create a parser object
   StimulusXmlParser stimulus_parser;

   doc.LoadFile( STIMULUS_FILE_NAME );
   ticpp::Node* root = doc.FirstChildElement( stimulus_parser.Name() );
   xml::NodeMap stimulus_map = stimulus_parser.Parse( root );

   const double XY_WIDTH = lexical_cast<int>( stimulus_map["xy_width"] );
   const double Z_WIDTH = lexical_cast<int>( stimulus_map["z_width"] );

   // create a CodeGenerator object. This is required to generate the
   // header file. 
   code_generator::CodeGenerator code_generator;

   // generate data output type
   code_generator.AddTypeDef(
         "XY_DATA_TYPE",
         "sc_int<" + lexical_cast< string >( XY_WIDTH ) + ">"
         );

   // generate z data output type
   code_generator.AddTypeDef(
         "Z_DATA_TYPE",
         "sc_int<" + lexical_cast< string >( Z_WIDTH ) + ">"
         );

   code_generator.AddTypeDef(
         "RESET_TYPE",
         "bool"
         );

   code_generator.AddInclude( "systemc.h", true );

   code_generator.GenerateFile( HEADER_FILE_NAME );

   return EXIT_SUCCESS;
}
