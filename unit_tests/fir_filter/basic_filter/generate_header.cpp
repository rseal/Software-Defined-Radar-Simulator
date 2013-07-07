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

#include <sdr_simulator/yaml/AccumulatorYaml.hpp>
#include <sdr_simulator/yaml/HalfBandYaml.hpp>
#include <sdr_simulator/util/CodeGenerator.hpp>
#include <sdr_simulator/yaml/NodeParser.hpp>

using namespace std;
using namespace code_generator;
using namespace boost;

int main(void)
{
   const std::string CONFIG_FILE_NAME = "sdr.yml";
   const string HEADER_FILE_NAME      = "test_bench.hpp";

	YAML::Node nodes = YAML::LoadFile( CONFIG_FILE_NAME );

   yaml::AccumulatorYamlPtr acc_node = yaml::NodeParser::ParseNode<yaml::AccumulatorYaml>(nodes,"accumulator");
   yaml::HalfBandYamlPtr hb_node = yaml::NodeParser::ParseNode<yaml::HalfBandYaml>(nodes,"half_band_filter");

   const unsigned int ACCUMULATOR_WIDTH = acc_node->bitWidth;
   const unsigned int COEFF_WIDTH       = hb_node->coeffWidth;
   const unsigned int INPUT_WIDTH       = hb_node->inputWidth;
   const unsigned int OUTPUT_WIDTH      = hb_node->outputWidth;

   // create a CodeGenerator object. This is required to generate the
   // header file. 
   code_generator::CodeGenerator code_generator;

   code_generator.AddTypeDef(
         "reset_type",
         "bool"
         );

   code_generator.AddConstant<int>(
         "ACCUMULATOR_WIDTH",
         ACCUMULATOR_WIDTH
         );

   code_generator.AddConstant<int>(
         "COEFF_WIDTH",
         COEFF_WIDTH
         );

   code_generator.AddConstant<int>(
         "INPUT_WIDTH",
         INPUT_WIDTH
         );

   code_generator.AddConstant<int>(
         "OUTPUT_WIDTH",
         OUTPUT_WIDTH
         );

   // generate data input type
   code_generator.AddTypeDef( "data_input_type", "sc_int<INPUT_WIDTH>");

   // generate data output type
   code_generator.AddTypeDef( "data_output_type", "sc_int<OUTPUT_WIDTH>");

   code_generator.AddInclude( "systemc.h", true );

   code_generator.GenerateFile( HEADER_FILE_NAME );

   return EXIT_SUCCESS;
}
