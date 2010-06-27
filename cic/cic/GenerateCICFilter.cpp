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
#include <fstream>
#include <iostream>
#include <vector>
#include <boost/lexical_cast.hpp>

#include <sdr_simulator/CicErrorAnalysis.hpp>

using namespace std;
using namespace boost;

void writeIncludes( ofstream& stream);
void writeBitWidths( ofstream& stream , vector<int>& vec);
void writeSignals( ofstream& stream, vector<int>& vec );
void writeStages( ofstream& stream, const int N, const vector<int>& vec);
void writeCTOR( ofstream& stream, const int N, const vector<int>& vec );
void writeCompute( ofstream& stream, const int N );
void writePorts( ofstream& stream );
void writeFinal( ofstream& stream );
const string createSharedPtr( 
      const string& className, 
      const string& instanceName,
      const int inputSize, 
      const int outputSize, 
      const int stageNumber, 
      const bool addPorts
      );
const string createTemplateName(
      const string& className, 
      const string& instanceName, 
      const int inputSize, 
      const int outputSize,
      const int stageNumber
      );

int main( void ) 
{

   ofstream outputFileStream( "Cic.hpp", ios::out );

   const int NUM_STAGES = 4;
   const int MAX_DECIMATION = 1024;
   const int DIFF_DELAY = 1;
   const int NUM_OUTPUT_BITS = 16;

   vector<int> bitTrimVector_;

   CicErrorAnalysis cea;
   bitTrimVector_ = cea.ComputeBitWidths(
         NUM_STAGES,
         MAX_DECIMATION,
         DIFF_DELAY,
         NUM_OUTPUT_BITS
         );

   int size = bitTrimVector_.size();

   for(int i=0; i< size; ++i){
      cout 
         << "stage " 
         << lexical_cast<string>(i) 
         << " = " 
         << bitTrimVector_[i] 
         << endl;
   }

   writeIncludes( outputFileStream );
   writeSignals( outputFileStream, bitTrimVector_ );
   writeStages( outputFileStream, NUM_STAGES, bitTrimVector_ );
   writeCompute( outputFileStream, NUM_STAGES);
   writeCTOR( outputFileStream, NUM_STAGES, bitTrimVector_ );
   writePorts( outputFileStream );
   writeFinal( outputFileStream );
   outputFileStream.close();

   return 0;
}

void writeIncludes( ofstream& stream )
{
   stream 
      << "#include<systemc.h>\n" 
      << "#include<iostream>\n\n"
      << "#include<boost/shared_ptr.hpp>\n"
      << "#include<sdr_simulator/CicIntegrator.hpp>\n"
      << "#include<sdr_simulator/CicDifferentiator.hpp>\n\n"
      << "template< unsigned int INPUT_WIDTH, unsigned int OUTPUT_WIDTH >\n"
      << "class Cic : public sc_module {\n\n";
}

void writeBitWidths( ofstream& stream , vector<int>& vec)
{
   int size = vec.size();

   for(int i=0; i<size; ++i) {
      stream 
         << "const int STAGE_" 
         << lexical_cast<string>(i)
         << "_WIDTH = " << vec[i] << ";\n";
   }
   stream << "\n";
}

void writeSignals( ofstream& stream, vector<int>& vec )
{
   const int MAX_INT_WIDTH = 64;
   int size = vec.size();

   string dataType = ( vec[0] > MAX_INT_WIDTH ) ? "sc_bigint< " : "sc_int< ";

   for(int i=0; i<size-1; ++i){

      stream 
         << "sc_signal< " 
         << dataType
         << vec[i] 
         << " > > sig_" 
         << i
         << "_;\n";
   }
}

void writeStages( ofstream& stream, const int N, const vector<int>& vec )
{
   for( int i =0; i<N; ++i)
      stream << createTemplateName( "CicIntegrator", "integrator", vec[i], vec[i+1], i) << ";\n";

   for( int i =N; i<2*N; ++i)
      stream << createTemplateName( "CicDifferentiator", "differentiator", vec[i], vec[i+1], i) << ";\n";

   stream << "\n";
}

const string createTemplateName(
      const string& className, 
      const string& instanceName, 
      const int inputSize, 
      const int outputSize,
      const int stageNumber
      )
{
   string result = "boost::shared_ptr<" + className + "< " + 
      lexical_cast<string>(inputSize) + ", " + lexical_cast<string>(outputSize) + 
      " > > " + instanceName + "_" + lexical_cast<string>(stageNumber) + "_";

   return result;
}

const string createSharedPtr( 
      const string& className, 
      const string& instanceName, 
      const int inputSize, 
      const int outputSize,
      const int stageNumber
      ) {

   string templateStr;
   string name;
   string result;

   string stage0 = lexical_cast<string>(stageNumber);
   string stage1 = lexical_cast<string>(stageNumber + 1);
   string size1 =lexical_cast<string>(inputSize); 
   string size2 =lexical_cast<string>(outputSize); 

   templateStr = className + "< " + size1 + "," + size2 + " >";
   name = instanceName + "_" + stage0 + "_";

   result = name + " = boost::shared_ptr<" + className + "< " + size1 + ", " + 
      size2 + " > >\n(\nnew " + templateStr + "( \"" + name + "\" )\n);\n";
   
   result+= name + "->clock( clock );\n";
   result+= name + "->reset( reset );\n";
   result+= name + "->input( sig_" + stage0 + "_ );\n";
   result+= name + "->output( sig_" + stage1 + "_ );\n";

   return result;
}

void writeCTOR( ofstream& stream, const int N, const vector<int>& vec )
{
      string portName;

      stream 
         << "typedef sc_uint<1> bit_type;\n"
         << "typedef sc_int< INPUT_WIDTH > data_input_type;\n"
         << "typedef sc_int< OUTPUT_WIDTH > data_output_type;\n\n";

      stream 
         << "public:\n\n"
         << "SC_HAS_PROCESS( Cic );\n\n"
         << "Cic( const sc_module_name& nm) : sc_module(nm){\n\n"
         << "SC_METHOD( Compute );\n"
         << "sensitive << clock.pos();\n\n"
         << createSharedPtr( "CicIntegrator", "integrator", vec[0], vec[1], 0 )
         << "\n";

      for( int i =1; i<N; ++i)
         stream << createSharedPtr( "CicIntegrator", "integrator", vec[i], vec[i+1], i ) << "\n";

      for( int i =N; i<2*N-1; ++i)
         stream << createSharedPtr( "CicDifferentiator", "differentiator", vec[i], vec[i+1], i ) << "\n";

      stream << "};\n\n";
}

void writePorts( ofstream& stream )
{
   stream 
      << "sc_in_clk clock;\n"
      << "sc_in< bit_type > reset;\n"
      << "sc_in< data_input_type > input;\n"
      << "sc_out< data_output_type > output;\n";
}

void writeCompute( ofstream& stream, const int N)
{
   stream
      << "void Compute(){\n"
      << "   sig_0_ = input.read();\n"
      << "   output = sig_"
      << (2*N-1)
      << "_.read();\n"
      << "};\n\n";
}

void writeFinal( ofstream& stream )
{
   stream << "};\n";
}


