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
         virtual NodeMap Parse( ticpp::Node* )=0;
         virtual std::string Name()=0;
   };
};

#endif
