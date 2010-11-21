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
