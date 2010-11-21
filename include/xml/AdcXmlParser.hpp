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
