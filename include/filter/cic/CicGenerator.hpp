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
#include <sdr_simulator/yaml/CicYaml.hpp>
#include <sdr_simulator/yaml/NodeParser.hpp>
#include <tr1/math.h>
#include <yaml-cpp/yaml.h>

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
    yaml::CicYamlPtr cicNode_;

public:

    CicGenerator( const std::string& configFileName ) {

		 const std::string CIC_MODULE = "cic_filter";

       // open configuration file and parse
		 YAML::Node config = YAML::LoadFile(configFileName);

       try
       {
			 cicNode_ = yaml::NodeParser::ParseNode<yaml::CicYaml>( config, CIC_MODULE);
       }
       catch ( YAML::ParserException& e )
       {
          std::cout << e.what();
       }


       //inputWidth_        = boost::lexical_cast<int>( config_map["input_width"] );
       //outputWidth_       = boost::lexical_cast<int>( config_map["output_width"] );
       //maxDecimation_     = boost::lexical_cast<int>( config_map["max_decimation"] );
       //cicNode_->numStages         = boost::lexical_cast<int>( config_map["num_stages"] );
       //differentialDelay_ = boost::lexical_cast<int>( config_map["differential_delay"] );
       //useBitPruning_     = config_map["bit_pruning"] == "false" ? false : true;

       std::cout
            << "input width    = " << cicNode_->inputWidth    << "\n"
            << "output width   = " << cicNode_->outputWidth   << "\n"
            << "max decimation = " << cicNode_->maxDecimation << "\n"
            << "num stages     = " << cicNode_->numStages     << "\n"
            << "bit pruning    = " << cicNode_->useBitPruning << "\n"
            << std::endl;

       outputStream_ = FileOutputStream(
             new std::ofstream( OUTPUT_FILE_NAME , std::ios::out )
             );

       CicErrorAnalysis cicErrorAnalysis;

       bitWidthVector_ = cicErrorAnalysis.ComputeBitWidths (
             cicNode_->numStages,
             cicNode_->minDecimation,
             cicNode_->maxDecimation,
             cicNode_->differentialDelay,
             cicNode_->inputWidth,
             cicNode_->outputWidth
             );

       int size = bitWidthVector_.size();

       // If bit pruning is not used, set bitwidth to the maximum size for 
       // every stage in the filter. 
       if( !cicNode_->useBitPruning )
       {
          for( int i=0; i<size; ++i )
          {
             bitWidthVector_[i] = cicNode_->inputWidth + 
                std::tr1::ceil( cicNode_->numStages*std::tr1::log2( cicNode_->maxDecimation ) );
          }
       }

       for ( int i = 0; i < size; ++i ) {
          std::cout << "s" << boost::lexical_cast<std::string> ( i ) << " | ";
       }
       std::cout << std::endl;

       for ( int i = 0; i < size; ++i ) {
          std::cout << bitWidthVector_[i] << " | ";
       }
       std::cout << std::endl;

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
          << "#include<tr1/math.h>\n\n"
          << "template< typename INPUT_TYPE, typename OUTPUT_TYPE >\n"
          << "class Cic : public sdr_module::Module< INPUT_TYPE, OUTPUT_TYPE > {\n\n"
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

       for ( int i = 0; i < SIZE; ++i ) {
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
       if(cicNode_->useBitPruning)
       {
          const int b = bitWidthVector_[0]-cicNode_->inputWidth+1;
          const double Rmax = std::tr1::pow(2.0,1.0*b/cicNode_->numStages)/cicNode_->differentialDelay;
          *outputStream_
             << "   virtual void Compute(){\n"
             << "      const double r = " << Rmax << "/decimation.read();\n"
             << "      const int bit_gain = std::tr1::ceil(" << cicNode_->numStages << "*std::tr1::log2(r));\n"
             << "      sc_int< " << bitWidthVector_[0] << "> buffer = this->input.read() << bit_gain-1;\n"
             << "      sig_0_.write( buffer );\n"
             << "}\n\n";
       }
       else
       {
          *outputStream_
             << "   virtual void Compute(){\n"
             << "      sc_int< " << bitWidthVector_[0] << "> buffer = this->input.read();\n"
             << "      sig_0_.write( buffer );\n"
             << "}\n\n";
       }
    }

    void writeComputeOutput() {

       const int SIZE        = bitWidthVector_.size();
       int LAST_STAGE_MSB    = bitWidthVector_[ SIZE - 1 ] - 1;
       int OUTPUT_STAGE_MSB  = cicNode_->outputWidth-1;
       int LAST_STAGE_NUMBER = 2*cicNode_->numStages;

       int msb = LAST_STAGE_MSB;
       int lsb = msb - cicNode_->outputWidth + 1;

       if( !cicNode_->useBitPruning )
       {
          *outputStream_
             << " void ComputeOutput(){\n"
             << " int user_decimation = decimation.read();\n"
             << " int bit_gain = std::tr1::ceil( "
             << cicNode_->numStages
             << "*std::tr1::log2( user_decimation ));\n"
             << " int msb = " << cicNode_->outputWidth << " + bit_gain -1;\n"
             << " int lsb = msb - " << cicNode_->outputWidth<< ";\n"
             << " sc_bv< " << cicNode_->outputWidth << " > output = sc_bv< "
             << cicNode_->outputWidth << ">( sig_" << LAST_STAGE_NUMBER
             << "_.read().range( msb , lsb ));\n"
             << " this->output = output.to_int();\n"
             << "}\n\n";
       }
       else
       {
          *outputStream_
             << "   void ComputeOutput(){\n"
             << "      int msb = " << msb << ";\n"
             << "      int lsb = " << lsb << ";\n"
             << "      sc_bv< " << cicNode_->outputWidth << " > output = sc_bv< " 
             << cicNode_->outputWidth << ">( sig_" << LAST_STAGE_NUMBER 
             << "_.read().range( msb , lsb ));\n"
             << "      this->output = output.to_int();\n"
             << "}\n\n";
       }
    }

    void writeDivClock() {

       const int SIZE        = bitWidthVector_.size();
       const int WIDTH       = bitWidthVector_[ SIZE - 2 ];
       const int NUM_MODULES = 2*cicNode_->numStages;

       *outputStream_
          << "   void DivClock(){\n"
          << "      if( ++idx_ == decimation.read()/2 )\n"
          << "      {\n"
          << "         idx_ = 0;\n"
          << "         div_clock_ = !div_clock_.read();\n"
          << "      }\n   }\n\n";
    }

    void writeStages() {

       for ( int i = 0; i < cicNode_->numStages; ++i ) {
          *outputStream_
             << createTemplateName ( "   CicIntegrator", "integrator",
                   bitWidthVector_[i], bitWidthVector_[i+1], i )
             << ";\n";
       }

       for ( int i = cicNode_->numStages; i < 2 * cicNode_->numStages; ++i ) {
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
          boost::lexical_cast<std::string> ( inputSize ) + 
          " , " +
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
          const int stageNumber,
          bool useDivClock = false
          ) {

       std::string templateStr;
       std::string name;
       std::string result;

       std::string stage0 = boost::lexical_cast<std::string> ( stageNumber );
       std::string stage1 = boost::lexical_cast<std::string> ( stageNumber + 1 );
       std::string size1  = boost::lexical_cast<std::string> ( inputSize );
       std::string size2  = boost::lexical_cast<std::string> ( outputSize );

       templateStr = className + "< " + size1 + " , " + size2 + " >";
       name = instanceName + "_" + stage0 + "_";

       result = name + " = boost::shared_ptr<" + className + "< " + 
          size1 + " , " + size2 + " > >\n      ( new " + templateStr + "( \"" + name + "\" ) );\n";

       if( useDivClock )
       {
          result += "      " + name + "->clock( this->div_clock_ );\n";
       }
       else
       {
          result += "      " + name + "->clock( this->clock );\n";
       }
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
          << "      sdr_module::Module< INPUT_TYPE, OUTPUT_TYPE>(nm){\n\n"
          << "      SC_METHOD( ComputeOutput );\n"
          << "      this->sensitive << div_clock_.posedge_event();\n\n"
          << "      SC_METHOD( DivClock );\n"
          << "      this->sensitive << this->clock.pos();\n"
          << "      div_clock( div_clock_ );\n\n"
          << "      "
          << createSharedPtr ( "CicIntegrator", "integrator",
                bitWidthVector_[0], bitWidthVector_[1], 0 )
          << "\n";

       for ( int i = 1; i < cicNode_->numStages; ++i ) {
          *outputStream_
             << "      "
             << createSharedPtr ( "CicIntegrator", "integrator",
                   bitWidthVector_[i], bitWidthVector_[i+1], i )
             << "\n";
       }

       for ( int i = cicNode_->numStages; i < 2 * cicNode_->numStages ; ++i ) {
          *outputStream_
             << "      "
             << createSharedPtr ( "CicDifferentiator", "differentiator",
                   bitWidthVector_[i], bitWidthVector_[i+1], i, true )
             << "\n";
       }

       *outputStream_ << "   };\n\n";
    }

    void writePorts () {
       *outputStream_
          << "   sc_in< INPUT_TYPE > decimation;\n\n"
          << "   clk_export_out div_clock;\n\n";
    }

    void writeFinal () {
       *outputStream_ << "};\n";
    }
};

#endif
