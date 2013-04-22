/*
 * id.h
 *
 *  Created on: 2012-5-11
 *      Author: vincent
 */

#ifndef NET_ID_H_
#define NET_ID_H_

#include <cstdlib> // for size_t

namespace atlasdb {
  namespace utility {

    /*
     * @Deplicated, use boost::uuid instead
     * */
    template<class Target>
    class id_generator {
    public:

      size_t next() {
        static size_t id = 0;
        //TODO: lock/atomic
        int current_id = id++;
        return current_id;
      }

    private:
    };
  } // utility
} // atlasdb

#endif /* ID_H_ */
