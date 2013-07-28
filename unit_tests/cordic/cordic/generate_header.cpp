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
#include <yaml-cpp/yaml.h>

#include <sdr_simulator/util/CodeGenerator.hpp>
#include <sdr_simulator/yaml/CordicYaml.hpp>
#include <sdr_simulator/yaml/AccumulatorYaml.hpp>
#include <sdr_simulator/yaml/NodeParser.hpp>

using namespace std;
using namespace code_generator;
using namespace boost;
using namespace yaml;

int main(int argc, char* argv[])
{
   const string HEADER_FILE_NAME        = "configuration.hpp";
   const std::string CONFIG_FILE_NAME   = "sdr.yml";

   YAML::Node nodes = YAML::LoadFile(CONFIG_FILE_NAME);

   CordicYamlPtr cordic_node = yaml::NodeParser::ParseNode<CordicYaml>(nodes,"ddc");

   // create a CodeGenerator object. This is required to generate the header file. 
   code_generator::CodeGenerator code_generator;

   code_generator.OpenNamespace("sdr"); // sdr namespace
   code_generator.AddTypeDef( "RESET_TYPE", "bool");
   code_generator.CloseNamespace(); // end sdr namespace

   code_generator.OpenNamespace("ddc"); // ddc namespace
   code_generator.AddConstant < int >  ( "DATA_WIDTH"  , cordic_node->xyWidth  )  ; 
   code_generator.AddConstant < int >  ( "PHASE_WIDTH" , cordic_node->zWidth )  ; 
   code_generator.AddConstant < int >  ( "NUM_STAGES" , cordic_node->numStages )  ; 
   code_generator.AddTypeDef ( "INPUT_TYPE"  , "sc_int < DATA_WIDTH  > " )  ; 
   code_generator.AddTypeDef ( "OUTPUT_TYPE" , "sc_int < DATA_WIDTH  > " )  ; 
   code_generator.AddTypeDef ( "PHASE_TYPE"  , "sc_int < PHASE_WIDTH > " )  ; 
   code_generator.CloseNamespace(); // end ddc namespace

   code_generator.OpenNamespace("accumulator");
   AccumulatorYamlPtr acc_node = yaml::NodeParser::ParseNode<AccumulatorYaml>(nodes,"accumulator");
   code_generator.AddConstant < int >  ( "INPUT_WIDTH", acc_node->inputWidth  )  ; 
   code_generator.AddConstant < int >  ( "OUTPUT_WIDTH" , acc_node->outputWidth )  ; 
   code_generator.AddTypeDef( "INPUT_TYPE", "sc_int<INPUT_WIDTH>");
   code_generator.AddTypeDef( "OUTPUT_TYPE", "sc_int<OUTPUT_WIDTH>");
   code_generator.CloseNamespace(); // end ddc namespace

   code_generator.AddInclude( "systemc.h", true );

   code_generator.GenerateFile( HEADER_FILE_NAME );

   return EXIT_SUCCESS;
}
