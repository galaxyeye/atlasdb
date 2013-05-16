/*
 * query.h
 *
 *  Created on: 2012-5-14
 *      Author: vincent
 */

#ifndef ATLASDB_NET_QUERY_H_
#define ATLASDB_NET_QUERY_H_

#include <memory>
#include <string>
#include <vector>

#include <atlasdb/net/response.h>

namespace atlasdb {
  namespace net {

    /**
     * @see session
     * */
    class query : public std::enable_shared_from_this<query> {
    public:

      size_t id() const { return _id; }
      size_t session_id() const { return _session_id; }

      session_ptr session() const { return _session; }
      void session(session_ptr session) { _session = session; }

      void execute() {

      }

    private:

      std::vector<char> _query_buffer;

      size_t _id;
      size_t _session_id;

      request_ptr _session;
      response_ptr _response;
    };

  } // net
} // atlasdb

#endif /* ATLASDB_NET_QUERY_H_ */
