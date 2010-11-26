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
#include <sdr_simulator/filter/cic/CicErrorAnalysis.hpp>
#include <sdr_simulator/xml/CicXmlParser.hpp>

namespace
{
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

    CicGenerator( const std::string& configFileName ) {

        ticpp::Document doc( configFileName );

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

        for ( int i = 0; i < size; ++i ) {
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

    void writeIncludes( ) {
        *outputStream_
                << "#include<systemc.h>\n"
                << "#include<iostream>\n\n"
                << "#include<boost/shared_ptr.hpp>\n"
                << "#include<sdr_simulator/filter/cic/CicIntegrator.hpp>\n"
                << "#include<sdr_simulator/filter/cic/CicDifferentiator.hpp>\n"
                << "#include<sdr_simulator/SdrModule.hpp>\n"
                << "#include \"test_bench.hpp\"\n\n"
                << "template< typename INPUT_DATA_TYPE, typename OUTPUT_DATA_TYPE >\n"
                << "class Cic : public sdr_module::Module<INPUT_DATA_TYPE,OUTPUT_DATA_TYPE> {\n\n"
                << "   typedef sc_export< sc_signal_inout_if<bool> > clk_export_out;\n"
                << "   sc_signal< bool > div_clock_;\n"
                << "   int idx_;\n\n";
    }

    void writeBitWidths() {
        const int SIZE = bitWidthVector_.size();

        for ( int i = 0; i < SIZE; ++i ) {
            *outputStream_
                    << "const int STAGE_"
                    << boost::lexical_cast<std::string> ( i )
                    << "_WIDTH = " << bitWidthVector_[i]
                    << ";\n";
        }

        *outputStream_ << "\n";
    }

    void writeSignals() {
        const int MAX_INT_WIDTH = 64;
        const int SIZE = bitWidthVector_.size();

        std::string dataType = ( bitWidthVector_[0] > MAX_INT_WIDTH ) ?  "sc_bigint< " : "sc_int< ";

        for ( int i = 0; i < SIZE-1; ++i ) {
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

    void writeComputeInput() {
        *outputStream_
                << "   virtual void Compute(){\n"
                << "      sc_int< " << bitWidthVector_[0] << "> buffer = this->input.read();\n"
                << "      buffer = buffer << "
                << bitWidthVector_[0] - numStages_ - inputWidth_ << ";\n"
                << "      sig_0_.write( buffer );\n   }\n\n";
    }

    void writeComputeOutput() {
        const int SIZE = bitWidthVector_.size();
        const int WIDTH = bitWidthVector_[ SIZE - 2 ];
        const int NUM_MODULES = 2*numStages_;

        *outputStream_
                << "   void ComputeOutput(){\n"
                << "      this->output = OUTPUT_DATA_TYPE( sc_int<" << WIDTH
                << ">( sig_" << NUM_MODULES << "_.read() ).range("
                << WIDTH-1 << "," << WIDTH-1-outputWidth_
                << ") );\n   }\n\n";
    }

    void writeDivClock() {
        *outputStream_
                << "   void DivClock(){\n"
                << "      if( idx_++ == (decimation.read() -1) )\n"
                << "      {\n"
                << "         idx_ = 0;\n"
                << "         div_clock_ = !div_clock_.read();\n"
                << "      }\n   }\n\n";
    }

    void writeStages() {

        for ( int i = 0; i < numStages_; ++i ) {
            *outputStream_
                    << createTemplateName ( "   CicIntegrator", "integrator",
                                            bitWidthVector_[i], bitWidthVector_[i+1], i )
                    << ";\n";
        }

        for ( int i = numStages_; i < 2 * numStages_; ++i ) {
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
    ) {
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
    ) {

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

        result += "      " + name + "->clock( this->clock );\n";
        result += "      " + name + "->reset( this->reset );\n";
        result += "      " + name + "->input( sig_" + stage0 + "_ );\n";
        result += "      " + name + "->output( sig_" + stage1 + "_ );\n";

        return result;
    }

    void writeCTOR () {
        std::string portName;

        *outputStream_
                << "   public:\n\n"
                << "      SC_HAS_PROCESS( Cic );\n\n"
                << "      Cic( const sc_module_name& nm) :\n"
                << "      sdr_module::Module< INPUT_DATA_TYPE,OUTPUT_DATA_TYPE>(nm){\n\n"
                << "      SC_METHOD( ComputeOutput );\n"
                << "      this->sensitive << div_clock_.posedge_event();\n\n"
                << "      SC_METHOD( DivClock );\n"
                << "      this->sensitive << this->clock.pos();\n"
                << "      div_clock( div_clock_ );\n\n"
                << "      "
                << createSharedPtr ( "CicIntegrator", "integrator",
                                     bitWidthVector_[0], bitWidthVector_[1], 0 )
                << "\n";

        for ( int i = 1; i < numStages_; ++i ) {
            *outputStream_
                    << "      "
                    << createSharedPtr ( "CicIntegrator", "integrator",
                                         bitWidthVector_[i], bitWidthVector_[i+1], i )
                    << "\n";
        }

        for ( int i = numStages_; i < 2 * numStages_ ; ++i ) {
            *outputStream_
                    << "      "
                    << createSharedPtr ( "CicDifferentiator", "differentiator",
                                         bitWidthVector_[i], bitWidthVector_[i+1], i )
                    << "\n";
        }

        *outputStream_ << "   };\n\n";
    }

    void writePorts () {
        *outputStream_
                << "   sc_in< INPUT_DATA_TYPE > decimation;\n\n"
                << "   clk_export_out div_clock;\n\n";
    }

    void writeFinal () {
        *outputStream_ << "};\n";
    }
};

#endif
