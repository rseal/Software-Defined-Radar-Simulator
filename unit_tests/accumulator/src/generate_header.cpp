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

#include <sdr_simulator/xml/AccumulatorXmlParser.hpp>
#include <sdr_simulator/util/CodeGenerator.hpp>

using namespace std;
using namespace code_generator;

int main(int argc, char* argv[])
{
   const std::string CONFIGURATION_FILE_NAME = argv[1];
   const string HEADER_FILE_NAME = "test_bench.hpp";

   // open the configuration file for parsing
   ticpp::Document doc( CONFIGURATION_FILE_NAME );

   // Create a parser object
   AccumulatorXmlParser parser;

   // Parse the xml file.
   doc.LoadFile();

   // Use the root node for reference.
   ticpp::Node* root = doc.FirstChild();

   // find the first module node in the xml file
   ticpp::Node* node = root->FirstChildElement( parser.Name() );

   // Retrieve a map containing accumulator keywords
   xml::NodeMap map = parser.Parse( node ); 

   // create constants that were read in from the xml file.
   const int BIT_WIDTH = boost::lexical_cast<int>( map["bit_width"] );
   const double SAMPLE_RATE = boost::lexical_cast<double>( map["sample_rate"] );
   const double TUNING_FREQ = boost::lexical_cast<double>( map["tuning_frequency"] );

   // create a CodeGenerator object. This is required to generate the
   // header file. 
   code_generator::CodeGenerator code_generator;

   // generate data output type
   code_generator.AddTypeDef(
         "data_output_type",
         "sc_int<" + boost::lexical_cast< string >( BIT_WIDTH ) + ">"
         );

   code_generator.AddTypeDef(
         "reset_type",
         "bool"
         );

   code_generator.AddConstant<int>(
         "BIT_WIDTH",
         BIT_WIDTH
         );

   code_generator.AddConstant<double>(
         "SAMPLE_RATE",
         SAMPLE_RATE
         );

   code_generator.AddConstant<double>(
         "TUNING_FREQUENCY",
         TUNING_FREQ
         );


   code_generator.AddInclude( "systemc.h", true );

   code_generator.GenerateFile( HEADER_FILE_NAME );

   return EXIT_SUCCESS;
}
