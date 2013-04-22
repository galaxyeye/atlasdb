/*
* ptree_printer.h
*
*  Created on: Oct 9, 2011
*      Author: vincent
*/

#ifndef QUERY_PTREE_PRINTER_H_
#define QUERY_PTREE_PRINTER_H_

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

// To validate a json document, go to http://jsonlint.com/
// and http://json.parser.online.fr/

/*************************************************
Print a tree in readable format like this:

root
-<level1-key, level1-value>
-<level1-key, level1-value>
--<level2-key, level2-value>
--<level2-key, level2-value>
---<level3-key, level3-value>
-<level1-key, level1-value>
-<level1-key, level1-value>

****************************************************/

namespace atlasdb {

  using namespace boost::property_tree;

  template<class PTree = ptree>
  class ptree_printer {
  public:

    void print(const PTree& pt, int level = 0) {
      if (level == 0) {
        std::cout << "<" << pt.data() << ">" << std::endl;
        print(pt, level + 1);
      }
      else {
        for (auto it = pt.begin(); it != pt.end(); ++it) {
          int l = level;
          while (l--) std::cout << "-";
          std::cout << "<" << it->first << ", " << it->second.data() << ">" << std::endl;

          print(it->second, level + 1);
        }
      }
    }
  };
}

#endif // QUERY_PTREE_PRINTER_H_
