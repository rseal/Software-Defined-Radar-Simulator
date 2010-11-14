#ifndef ACCUMULATOR_XML_PARSER_HPP
#define ACCUMULATOR_XML_PARSER_HPP

#include <sdr_simulator/xml/IXmlParser.hpp>

class AccumulatorXmlParser : public xml::IXmlParser
{
   std::string name_;
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

   AccumulatorXmlParser(): name_("accumulator") {}

   const xml::NodeMap Parse( ticpp::Node* node )
   {
      Insert( node, "module_name" );
      Insert( node, "bit_width" );
      Insert( node, "sample_rate" );
      Insert( node, "tuning_frequency" );

      return map_;
   }

   const std::string& Name() { return name_; }
};

#endif
