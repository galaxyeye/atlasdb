/**
 * request.h
 *
 *  Created on: 2012-5-11
 *      Author: vincent
 */

#ifndef ATLASDB_NET_REQUEST_H_
#define ATLASDB_NET_REQUEST_H_

#include <memory>
#include <queue>

#include <boost/asio/streambuf.hpp>

#include <atlasdb/net/query.h>

namespace atlasdb {
  namespace net {

    class request : public std::enable_shared_from_this<request> {
    public:

      request(const char* message, size_t size) : _message(message, size) {}

      ~request() {}

    public:

      session_ptr session() const { return _session; }
      void session(session_ptr session) { _session = session; }

    private:

      boost::asio::streambuf _message;

      std::queue<query_ptr> _queries;
      session_ptr _session;
      response_ptr _response;
    };

    typedef std::shared_ptr<request> request_ptr;

  } // db
} // atlasdb

#endif /* ATLASDB_NET_REQUEST_H_ */
