#ifndef DOWN_CONVERTER_XML_PARSER_HPP
#define DOWN_CONVERTER_XML_PARSER_HPP

#include <sdr_simulator/xml/IXmlParser.hpp>

class DownConverterXmlParser : public xml::IXmlParser
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
      Insert( node, "model" );
      Insert( node, "num_stages" );
      Insert( node, "xy_width" );
      Insert( node, "z_width" );

      return map_;
   }

   std::string Name() { return "down_converter"; }
};

#endif
