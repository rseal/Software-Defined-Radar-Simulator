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


   const static std::string OPEN_TEMPLATE = "<";
   const static std::string CLOSED_TEMPLATE = ">";
   const static std::string SPACE = " ";
   const static std::string TYPEDEF = "typedef";
   const static std::string NEW_LINE = "\n";
   const static std::string CPP_DELIM = ";";
   const static std::string CONST = "const";
   const static std::string STATIC = "static";
   const static std::string EQUALS = "=";
   const static std::string PERIOD = ".";
   const static std::string UNDER_SCORE = "_";
   const static std::string QUOTE = "\"";

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
               CONST + SPACE + STATIC + SPACE + data_type + SPACE + 
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
