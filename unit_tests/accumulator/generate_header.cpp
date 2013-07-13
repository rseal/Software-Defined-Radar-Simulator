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

#include <sdr_simulator/util/CodeGenerator.hpp>
#include <sdr_simulator/yaml/NodeParser.hpp>
#include <sdr_simulator/yaml/AccumulatorYaml.hpp>
#include <sdr_simulator/yaml/CordicYaml.hpp>
#include <boost/shared_ptr.hpp>
#include <tr1/math.h>

using namespace std;
using namespace code_generator;
using namespace yaml;

int main(void)
{
   const string HEADER_FILE_NAME             = "test_bench.hpp";
   const std::string CONFIGURATION_FILE_NAME = "sdr.yml";
   const std::string ACCUMULATOR_MODULE      = "accumulator";
   const std::string CORDIC_MODULE           = "down_converter";

   try
   {
      YAML::Node config = YAML::LoadFile(CONFIGURATION_FILE_NAME);

      CordicYamlPtr cordic_node = NodeParser::ParseNode<CordicYaml>( config, CORDIC_MODULE);
      AccumulatorYamlPtr acc_node = NodeParser::ParseNode<AccumulatorYaml>( config, ACCUMULATOR_MODULE);

      // create a CodeGenerator object. This is required to generate the
      // header file. 
      code_generator::CodeGenerator code_generator;

      code_generator.OpenNamespace("accumulator");

      code_generator.AddConstant<int>(
            "INPUT_WIDTH",
            acc_node->inputWidth
            );

      code_generator.AddConstant<int>(
            "OUTPUT_WIDTH",
            acc_node->outputWidth
            );

      // generate data output type
      code_generator.AddTypeDef(
            "INPUT_TYPE",
            "sc_uint<INPUT_WIDTH>"
            );

      code_generator.AddTypeDef(
            "OUTPUT_TYPE",
            "sc_uint<OUTPUT_WIDTH>"
            );

      code_generator.AddTypeDef(
            "reset_type",
            "bool"
            );

      code_generator.AddConstant<double>(
            "SAMPLE_RATE",
            acc_node->sampleRate
            );

      code_generator.AddConstant<double>(
            "TUNING_FREQUENCY",
            cordic_node->tuningFrequency
            );

      code_generator.AddConstant<double>(
            "STEP_SIZE",
            rint( std::tr1::pow(2.0,acc_node->inputWidth) * cordic_node->tuningFrequency / acc_node->sampleRate )
            );

      code_generator.CloseNamespace("accumulator");

      code_generator.AddInclude( "systemc.h", true );

      code_generator.GenerateFile( HEADER_FILE_NAME );
   }
   catch ( YAML::ParserException& e )
   {
      std::cout << e.what();
   }

   return EXIT_SUCCESS;
}
