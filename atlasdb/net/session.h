/*
 * session.h
 *
 *  Created on: Mar 16, 2012
 *      Author: vincent
 */

#ifndef ATLASDB_NET_SESSION_H_
#define ATLASDB_NET_SESSION_H_

#include <boost/uuid/uuid.hpp>

#include <atlas/container/concurrent_box.h>

#include <atlasdb/net/request.h>

namespace atlasdb {
  namespace net {

    using boost::uuids::uuid;

    /*
     * Class Relationship:
     *
     *          has a                   has many
     * Session ------------> Request ------------> Query
     *         <------------         <------------
     *          belongs to              belongs to
     *
     * */
    class session: public std::enable_shared_from_this<session> {
    public:

      session() {}

      explicit session(const uuid& id) : _id(id) {}

      const uuid& id() const { return _id; }

      request_ptr request() const { return _request; }

    private:

      uuid _id;
      request_ptr _request;
    };

  } // net
} // atlasdb

#endif /* ATLASDB_NET_SESSION_H_ */
