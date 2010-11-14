#ifndef I_XML_PARSER_HPP
#define I_XML_PARSER_HPP

#include<map>
#include<iostream>
#include<ticpp/ticpp.h>

namespace xml
{
   typedef std::map< std::string, std::string> NodeMap;

   class IXmlParser
   {
      public:

         // virtual Parse method
         virtual const NodeMap Parse( ticpp::Node* )=0;
         virtual const std::string& Name()=0;
   };
};

#endif
