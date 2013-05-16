/*
 * connection_manager.h
 *
 *  Created on: May 1, 2013
 *      Author: vincent
 */

#ifndef ATLASDB_NET_CONNECTION_H_
#define ATLASDB_NET_CONNECTION_H_

#include <string>

#include <boost/asio/ip/tcp.hpp>

#include <atlas/container/blocking_concurrent_box.h>
#include <atlas/singleton.h>

namespace atlasdb {
  namespace net {

    namespace asio = boost::asio;
    using asio::ip::tcp;

    class connection_pool : public atlas::singleton<connection_pool> {
    public:

      void put(const connection_ptr& conn) {
        _connections.put(conn->socket().local_endpoint(), conn);
      }

      connection_ptr take(const tcp::endpoint& ep) {
        boost::optional<connection> conn = _connections.take(ep);

        if (conn) return conn.get();
        return nullptr;
      }

      void erase(const tcp::endpoint& ep) { _connections.erase(ep); }

      void clear() { _connections.clear(); }

    private:

      friend class atlas::singleton<connection_pool>;
      connection_pool() = default;
      ~connection_pool() = default;

    private:

      atlas::blocking_concurrent_box<tcp::endpoint, connection_ptr> _connections;
    };

  } // net
} // atlasdb

#endif /* ATLASDB_NET_CONNECTION_MANAGER_H_ */
