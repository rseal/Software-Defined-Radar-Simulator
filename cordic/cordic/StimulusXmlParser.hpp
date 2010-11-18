#ifndef STIMULUS_XML_PARSER_HPP
#define STIMULUS_XML_PARSER_HPP

#include <sdr_simulator/xml/IXmlParser.hpp>

class StimulusXmlParser : public xml::IXmlParser
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

   StimulusXmlParser(): name_("stimulus") {}

   const xml::NodeMap Parse( ticpp::Node* node )
   {
      Insert( node, "signal_frequency" );
      Insert( node, "ddc_frequency" );
      Insert( node, "sample_rate" );

      return map_;
   }

   const std::string& Name() { return name_; }
};

#endif
