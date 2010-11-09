#ifndef CIC_XML_PARSER_HPP
#define CIC_XML_PARSER_HPP

#include <sdr_simulator/xml/IXmlParser.hpp>

class CicXmlParser : public xml::IXmlParser
{
   xml::NodeMap map_;

   void Insert( ticpp::Node* node, const std::string& value )
   {
      map_.insert
         ( 
          std::pair< std::string, std::string >( 
             value,
             node->FirstChildElement( value )->GetText() 
             )
         );
   }

   public:

   xml::NodeMap Parse( ticpp::Node* node )
   {
      Insert( node, "module_name" );
      Insert( node, "input_width" );
      Insert( node, "output_width" );
      Insert( node, "decimation" );
      Insert( node, "num_stages" );

      return map_;
   }

   std::string Name() { return "cic"; }

};

#endif
