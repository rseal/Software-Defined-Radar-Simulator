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
#include <sdr_simulator/util/CodeGenerator.hpp>
#include <sdr_simulator/yaml/CicYaml.hpp>
#include <sdr_simulator/yaml/NodeParser.hpp>

using namespace std;
using namespace code_generator;
using namespace boost;
using namespace yaml;

int main(int argc, char* argv[])
{
   const std::string CONFIGURATION_FILE_NAME = "sdr.yml";
   const string HEADER_FILE_NAME             = "test_bench.hpp";
   const string CIC_FILE_NAME                = "Cic.hpp";
   const std::string CIC_MODULE              = "cic_filter";

   YAML::Node config = YAML::LoadFile(CONFIGURATION_FILE_NAME);

   CicYamlPtr cic_node = NodeParser::ParseNode<CicYaml>( config, CIC_MODULE);

   // create a CodeGenerator object. This is required to generate the
   // header file. 
   code_generator::CodeGenerator code_generator;

   code_generator.OpenNamespace("cic");
   // generate data input type
   code_generator.AddTypeDef(
         "INPUT_TYPE",
         "sc_int<" + lexical_cast< string >( cic_node->inputWidth ) + ">"
         );

   // generate data output type
   code_generator.AddTypeDef(
         "OUTPUT_TYPE",
         "sc_int<" + lexical_cast< string >( cic_node->outputWidth ) + ">"
         );

   code_generator.AddTypeDef(
         "RESET_TYPE",
         "bool"
         );

   code_generator.AddConstant<int>(
         "INPUT_WIDTH",
         cic_node->inputWidth
         );

   code_generator.AddConstant<int>(
         "OUTPUT_WIDTH",
         cic_node->outputWidth
         );

   code_generator.CloseNamespace("cic");

   code_generator.AddInclude( "systemc", true );

   code_generator.GenerateFile( HEADER_FILE_NAME );

   CicGenerator cicGenerator( CONFIGURATION_FILE_NAME );

   return EXIT_SUCCESS;
}
