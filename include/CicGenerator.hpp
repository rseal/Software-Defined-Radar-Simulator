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
#ifndef CIC_GENERATOR_HPP
#define CIC_GENERATOR_HPP

#include <fstream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <sdr_simulator/CicErrorAnalysis.hpp>
#include <sdr_simulator/xml/CicXmlParser.hpp>

namespace
{
   const static std::string CONFIGURATION_FILE_NAME = "../../config/sdr_config.xml";
   const static char* OUTPUT_FILE_NAME = "Cic.hpp";
}

class CicGenerator
{
  typedef boost::shared_ptr<std::ofstream> FileOutputStream;
  typedef std::vector<int> BitWidthVector;
  FileOutputStream outputStream_;
  BitWidthVector bitWidthVector_;

  int inputWidth_;
  int outputWidth_;
  int maxDecimation_;
  int numStages_;
  int differentialDelay_;

public:

  CicGenerator()
  {
    ticpp::Document doc( CONFIGURATION_FILE_NAME );

    CicXmlParser parser;

    doc.LoadFile();

    // Use the root node for reference.
    ticpp::Node* root = doc.FirstChild();

    // find the first module node in the xml file
    ticpp::Node* node = root->FirstChildElement( parser.Name() );

    // Retrieve a map containing accumulator keywords
    xml::NodeMap config_map = parser.Parse( node );

    inputWidth_ = boost::lexical_cast<int>( config_map["input_width"] ); 
    outputWidth_ = boost::lexical_cast<int>( config_map["output_width"] ); 
    maxDecimation_ = boost::lexical_cast<int>( config_map["max_decimation"] ); 
    numStages_ = boost::lexical_cast<int>( config_map["num_stages"] ); 
    differentialDelay_ = boost::lexical_cast<int>( config_map["differential_delay"] ); 

    std::cout 
       << " input width    = " << inputWidth_ << "\n"
       << " output width   = " << outputWidth_ << "\n"
       << " max decimation = " << maxDecimation_ << "\n"
       << " num stages     = " << numStages_ << "\n"
       << std::endl;

    outputStream_ = FileOutputStream(
                      new std::ofstream( OUTPUT_FILE_NAME , std::ios::out )
                    );


    CicErrorAnalysis cicErrorAnalysis;

    bitWidthVector_ = cicErrorAnalysis.ComputeBitWidths (
                        numStages_,
                        maxDecimation_,
                        differentialDelay_,
                        outputWidth_
                      );

    int size = bitWidthVector_.size();

    for ( int i = 0; i < size; ++i )
      {
        std::cout
            << "stage "
            << boost::lexical_cast<std::string> ( i )
            << " = "
            << bitWidthVector_[i]
            << std::endl;
      }

    writeIncludes ();
    writeSignals ();
    writeComputeInput();
    writeComputeOutput();
    writeDivClock();
    writeStages ();
    writeCTOR ();
    writePorts ();
    writeFinal ();

    outputStream_->close();
  }

  void writeIncludes( )
  {
    *outputStream_
        << "#include<systemc.h>\n"
        << "#include<iostream>\n\n"
        << "#include<boost/shared_ptr.hpp>\n"
        << "#include<sdr_simulator/CicIntegrator.hpp>\n"
        << "#include<sdr_simulator/CicDifferentiator.hpp>\n"
        << "#include \"test_bench.hpp\"\n\n"
        << "template< unsigned int INPUT_WIDTH, unsigned int OUTPUT_WIDTH >\n"
        << "class Cic : public sc_module {\n\n"
        << "   sc_signal< bool > div_clock;\n"
        << "   int idx_;\n\n";
  }

  void writeBitWidths()
  {
    const int SIZE = bitWidthVector_.size();

    for ( int i = 0; i < SIZE; ++i )
      {
        *outputStream_
            << "const int STAGE_"
            << boost::lexical_cast<std::string> ( i )
            << "_WIDTH = " << bitWidthVector_[i]
            << ";\n";
      }

    *outputStream_ << "\n";
  }

  void writeSignals()
  {
    const int MAX_INT_WIDTH = 64;
    const int SIZE = bitWidthVector_.size();

    std::string dataType = ( bitWidthVector_[0] > MAX_INT_WIDTH ) ?  "sc_bigint< " : "sc_int< ";

    for ( int i = 0; i < SIZE-1; ++i )
      {
        *outputStream_
            << "   sc_signal< "
            << dataType
            << bitWidthVector_[i]
            << " > > sig_"
            << i
            << "_;\n";
      }
    *outputStream_ << "\n\n";

  }

  void writeComputeInput()
  {
     *outputStream_
        << "   void ComputeInput(){\n"
        << "      sc_int< " << bitWidthVector_[0] << "> buffer = input.read();\n"
        << "      buffer = buffer << " 
        << bitWidthVector_[0] - numStages_ - inputWidth_ << ";\n"
        << "      sig_0_.write( buffer );\n   }\n\n";
  }

  void writeComputeOutput()
  {
     const int SIZE = bitWidthVector_.size();
     const int WIDTH = bitWidthVector_[ SIZE - 2 ];
     const int NUM_MODULES = 2*numStages_;

     *outputStream_
        << "   void ComputeOutput(){\n"
        << "      output = data_output_type( sc_int<" << WIDTH 
        << ">( sig_" << NUM_MODULES << "_.read() ).range(" 
        << WIDTH-1 << "," << WIDTH-1-outputWidth_ 
        << ") );\n   }\n\n";
  }

  void writeDivClock()
  {
     *outputStream_
        << "   void DivClock(){\n"
        << "      if( idx_++ == (decimation.read() -1) )\n"
        << "      {\n"
        << "         idx_ = 0;\n"
        << "         div_clock = !div_clock.read();\n"
        << "      }\n   }\n\n";
  }

  void writeStages()
  {

    for ( int i = 0; i < numStages_; ++i )
      {
        *outputStream_
            << createTemplateName ( "   CicIntegrator", "integrator",
                                    bitWidthVector_[i], bitWidthVector_[i+1], i )
            << ";\n";
      }

    for ( int i = numStages_; i < 2 * numStages_; ++i )
      {
        *outputStream_
            << createTemplateName ( "   CicDifferentiator", "differentiator",
                                    bitWidthVector_[i], bitWidthVector_[i+1], i )
            << ";\n";
      }

    *outputStream_ << "\n";
  }

  const std::string createTemplateName (
    const std::string& className,
    const std::string& instanceName,
    const int inputSize,
    const int outputSize,
    const int stageNumber
  )
  {
    std::string result = "   boost::shared_ptr<" + className + "< " +
                         boost::lexical_cast<std::string> ( inputSize ) + ", " +
                         boost::lexical_cast<std::string> ( outputSize ) +
                         " > > " + instanceName + "_" +
                         boost::lexical_cast<std::string> ( stageNumber ) + "_";

    return result;
  }

  const std::string createSharedPtr (
    const std::string& className,
    const std::string& instanceName,
    const int inputSize,
    const int outputSize,
    const int stageNumber
  )
  {

    std::string templateStr;
    std::string name;
    std::string result;

    std::string stage0 = boost::lexical_cast<std::string> ( stageNumber );
    std::string stage1 = boost::lexical_cast<std::string> ( stageNumber + 1 );
    std::string size1 = boost::lexical_cast<std::string> ( inputSize );
    std::string size2 = boost::lexical_cast<std::string> ( outputSize );

    templateStr = className + "< " + size1 + "," + size2 + " >";
    name = instanceName + "_" + stage0 + "_";

    result = name + " = boost::shared_ptr<" + className + "< " + size1 + ", " +
             size2 + " > >( new " + templateStr + "( \"" + name + "\" ) );\n";

    result += "      " + name + "->clock( clock );\n";
    result += "      " + name + "->reset( reset );\n";
    result += "      " + name + "->input( sig_" + stage0 + "_ );\n";
    result += "      " + name + "->output( sig_" + stage1 + "_ );\n";

    return result;
  }

  void writeCTOR ()
  {
    std::string portName;

    *outputStream_
        << "   public:\n\n"
        << "      SC_HAS_PROCESS( Cic );\n\n"
        << "      Cic( const sc_module_name& nm) : sc_module(nm){\n\n"
        << "      SC_METHOD( ComputeInput );\n"
        << "      sensitive << clock.pos();\n\n"
        << "      SC_METHOD( ComputeOutput );\n"
        << "      sensitive << div_clock.posedge_event();\n\n"
        << "      SC_METHOD( DivClock );\n"
        << "      sensitive << clock.pos();\n\n"
        << "      " 
        << createSharedPtr ( "CicIntegrator", "integrator",
              bitWidthVector_[0], bitWidthVector_[1], 0 )
        << "\n";

    for ( int i = 1; i < numStages_; ++i )
      {
        *outputStream_
            << "      " 
            << createSharedPtr ( "CicIntegrator", "integrator",
                  bitWidthVector_[i], bitWidthVector_[i+1], i )
            << "\n";
      }

    for ( int i = numStages_; i < 2 * numStages_ ; ++i )
      {
        *outputStream_
            << "      "
            << createSharedPtr ( "CicDifferentiator", "differentiator",
                  bitWidthVector_[i], bitWidthVector_[i+1], i )
            << "\n";
      }

    *outputStream_ << "   };\n\n";
  }

  void writePorts ()
  {
    *outputStream_
        << "   sc_in_clk clock;\n"
        << "   sc_in< reset_type > reset;\n"
        << "   sc_in< data_input_type > input;\n"
        << "   sc_out< data_output_type > output;\n"
        << "   sc_in< data_input_type > decimation;\n\n";
  }

  void writeFinal ()
  {
    *outputStream_ << "};\n";
  }
};

#endif
