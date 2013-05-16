/*
* storage_base.h
*
*  Created on: Apri 6, 2013
*      Author: vincent
*/

#ifndef ATLASDB_STORAGE_STORAGE_BASE_H_
#define ATLASDB_STORAGE_STORAGE_BASE_H_

#include <string>

namespace atlasdb {
  namespace storage {

    using std::string;

    class storage_base {

    public:

      storage_base() {}

      storage_base(const std::string& name) : _name(name) {}

    public:

      const std::string& name() const { return _name; }

    protected:

      void name(const std::string& name) { _name = name; }

    private:

      std::string _name;
    };

  } // storage
} // atlasdb

#endif /* ATLASDB_STORAGE_STORAGE_BASE_H_ */
