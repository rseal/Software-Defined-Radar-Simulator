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
#ifndef CIC_XML_PARSER_HPP
#define CIC_XML_PARSER_HPP

#include <sdr_simulator/xml/IXmlParser.hpp>

class CicXmlParser : public xml::IXmlParser
{
   const std::string name_;
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

   CicXmlParser(): name_("cic"){}

   const xml::NodeMap Parse( ticpp::Node* node )
   {
      Insert( node, "module_name" );
      Insert( node, "input_width" );
      Insert( node, "output_width" );
      Insert( node, "min_decimation" );
      Insert( node, "max_decimation" );
      Insert( node, "differential_delay");
      Insert( node, "num_stages" );
      Insert( node, "bit_pruning" );

      return map_;
   }

   const std::string& Name() { return name_; }

};

#endif
