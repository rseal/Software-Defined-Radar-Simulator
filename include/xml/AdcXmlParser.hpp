#ifndef ADC_XML_PARSER_HPP
#define ADC_XML_PARSER_HPP

#include <sdr_simulator/xml/IXmlParser.hpp>

class AdcXmlParser : public xml::IXmlParser
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
         Insert( node, "bit_width" );
         Insert( node, "model" );

         return map_;
      }

      std::string Name() { return "adc"; }
};

#endif
