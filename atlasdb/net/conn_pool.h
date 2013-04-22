/*
 * conn_pool.h
 *
 *  Created on: Apr 18, 2013
 *      Author: vincent
 */

#ifndef atlasdb_NET_CONN_POOL_H_
#define atlasdb_NET_CONN_POOL_H_

#include <boost/optional.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <atlasdb/net/connection.h>

namespace atlasdb {
  namespace net {

    namespace asio = boost::asio;
    using asio::ip::tcp::endpoint;

    class conn_pool {
    public:

      void put(const endpoint& ep, const connection_ptr& c) {
        _connections.insert(std::make_pair(ep, c));
      }

      boost::optional<connection_ptr> get(const endpoint& ep) {

        return boost::none_t();
      }

      connection_ptr take(const endpoint& ep) {
        return boost::none_t();
      }

    private:

      std::map<endpoint, connection_ptr> _connections;
    };

  } // net
} // atlasdb

#endif /* CONN_POOL_H_ */
