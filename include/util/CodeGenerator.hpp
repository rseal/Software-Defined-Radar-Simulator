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
#ifndef CODE_GENERATOR_HPP
#define CODE_GENERATOR_HPP

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <map>

namespace code_generator{

   enum { SC_INT, SC_BIG_INT, INT, DOUBLE };

   const std::string OPEN_TEMPLATE = "<";
   const std::string CLOSED_TEMPLATE = ">";
   const std::string SPACE = " ";
   const std::string TYPEDEF = "typedef";
   const std::string NEW_LINE = "\n";
   const std::string CPP_DELIM = ";";
   const std::string CONST = "const";
   const std::string STATIC = "static";
   const std::string EQUALS = "=";
   const std::string PERIOD = ".";
   const std::string UNDER_SCORE = "_";
   const std::string QUOTE = "\"";

   class CodeGenerator
   {
      typedef std::vector< std::string > List;
      List codeList_;
      List includeList_;
      typedef std::map< std::string, std::string > TypeMap;
      TypeMap typeMap_;

      public:

      CodeGenerator()
      {
         typeMap_[ typeid(int).name() ] = "int";
         typeMap_[ typeid(double).name() ] = "double";
         typeMap_[ typeid(float).name() ] = "float";
      }


      template< typename T >
         void AddConstant( const std::string& name, const T& value) {

            std::string result;

            TypeMap::iterator iter = typeMap_.find( typeid(T).name() );
            const std::string data_type = iter->second;

            result = 
               CONST + SPACE + SPACE + data_type + SPACE + 
               name + SPACE + EQUALS + SPACE + 
               boost::lexical_cast<std::string>( value ) + CPP_DELIM;

            codeList_.push_back( result );
         }

      template< typename T>
         void AddDataType(
               const std::string& name,
               const int& dataType,
               const T& value)
         {
            std::string result;

            TypeMap::iterator iter = typeMap_.find( typeid(T).name() );
            const std::string data_type = iter->second;

            result = 
               CONST + SPACE + data_type + SPACE + name + SPACE + EQUALS + 
               SPACE + boost::lexical_cast< std::string>( value ) + CPP_DELIM;

            codeList_.push_back( result );
         }

      void AddTypeDef( const std::string& name, const std::string& type )
      {
         std::string result;

         result = TYPEDEF + SPACE + type + SPACE + name + CPP_DELIM;

         codeList_.push_back( result );
      }

      void OpenNamespace( const std::string& name )
      {
         std::string result;
         result = "namespace " + name + "{";

         codeList_.push_back( result );
      }

      void CloseNamespace() 
      {
         codeList_.push_back("};");
      }

      void AddInclude( const std::string& name, const bool systemFile=true )
      {

         std::string includeStr = "#include ";

         if( systemFile )
         {
            includeStr += OPEN_TEMPLATE + name + CLOSED_TEMPLATE;
         }
         else
         {
            includeStr += QUOTE + name + QUOTE;
         }

         includeList_.push_back( includeStr );
      }

      void GenerateFile( const std::string& fileName )
      {

         std::string header_guard = fileName;
         boost::replace_first( header_guard, PERIOD, UNDER_SCORE );
         boost::to_upper( header_guard );


         std::ofstream fileOut( fileName.c_str(), std::ios::out );

         List::iterator iter = codeList_.begin();

         fileOut 
            << "#ifndef " << header_guard << NEW_LINE
            << "#define " << header_guard << NEW_LINE
            << NEW_LINE;

         List::iterator includeIter = includeList_.begin();

         while( includeIter != includeList_.end() )
         {
            fileOut << *includeIter++ << NEW_LINE;
         }
         fileOut << NEW_LINE;

         while( iter != codeList_.end() )
         {
            fileOut << *iter++ << std::endl;
         }

         fileOut << NEW_LINE << "#endif" << NEW_LINE;
         fileOut.close();
      }

   };
};

#endif
