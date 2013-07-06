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
#include <sdr_simulator/yaml/NodeParser.hpp>

using namespace std;
using namespace code_generator;
using namespace boost;
using namespace yaml;

int main(int argc, char* argv[])
{
   const std::string STIMULUS_FILE_NAME = "stimulus.yml";
   const string HEADER_FILE_NAME        = "test_bench.hpp";
   const std::string CONFIG_FILE_NAME   = "sdr.yml";

   YAML::Node nodes = YAML::LoadFile(CONFIG_FILE_NAME);

   CordicYamlPtr cordic_node = yaml::NodeParser::ParseNode<CordicYaml>(nodes,"down_converter");


   // create constants that were read in from the xml file.
   const int DATA_WIDTH  = cordic_node->xyWidth;
   const int PHASE_WIDTH = cordic_node->zWidth;
   //const int NUM_STAGES  = cordic_node->numStages;

   YAML::Node stim_node = YAML::LoadFile(STIMULUS_FILE_NAME)["stimulus"];

   const double SIGNAL_FREQUENCY = stim_node["signal_frequency"].as<double>();
   const double DDC_FREQUENCY    = stim_node["ddc_frequency"].as<double>();
   const double SAMPLE_RATE      = stim_node["sample_rate"].as<double>();

   const double NORMALIZED_FREQUENCY = SIGNAL_FREQUENCY/SAMPLE_RATE;
   const double SCALE                = tr1::pow(2.0, PHASE_WIDTH);
   const int ACCUMULATOR_STEP_SIZE   = rint( SCALE*DDC_FREQUENCY/SAMPLE_RATE );

   // create a CodeGenerator object. This is required to generate the
   // header file. 
   code_generator::CodeGenerator code_generator;

   code_generator.OpenNamespace("cordic");

   // generate data input type
   code_generator.AddTypeDef(
         "INPUT_TYPE",
         "sc_int<" + lexical_cast< string >( DATA_WIDTH ) + ">"
         );

   // generate data output type
   code_generator.AddTypeDef(
         "OUTPUT_TYPE",
         "sc_int<" + lexical_cast< string >( DATA_WIDTH ) + ">"
         );

   // generate z data output type
   code_generator.AddTypeDef(
         "PHASE_TYPE",
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

   //code_generator.AddConstant<int>(
         //"NUM_STAGES",
         //NUM_STAGES
         //);

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

   code_generator.CloseNamespace("cordic");

   code_generator.AddInclude( "systemc.h", true );

   code_generator.GenerateFile( HEADER_FILE_NAME );

   return EXIT_SUCCESS;
}
