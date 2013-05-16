/*
 * client.h
 *
 *  Created on: Apr 8, 2013
 *      Author: vincent
 */

#ifndef ATLASDB_NET_CLIENT_H_
#define ATLASDB_NET_CLIENT_H_

#include <string>
#include <functional>

#include <boost/asio/ip/udp.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>

#include <atlasdb/net/net_fwd.h>
#include <atlasdb/net/connection.h>

namespace atlasdb {
  namespace net {

    using asio::ip::udp;
    using asio::ip::tcp;

    class tcp_client {
    public:

      /// Constructor starts the asynchronous connect operation.
      tcp_client(asio::io_service& io_service, const std::string& host, const std::string& service) {
        // Resolve the host name into an IP address.
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(host, service);
        tcp::resolver::iterator it = resolver.resolve(query);

        connection_ptr c(new connection(io_service));

        // Start an asynchronous connect operation.
        auto handler = std::bind(&tcp_client::handle_connect, this, c, asio::placeholders::error);
        asio::async_connect(c->socket(), it, handler);
      }

      /// Handle completion of a connect operation.
      void handle_connect(const connection_ptr& c, const std::error_code& e) {
        if (e) {
          // An error occurred. Log it and return. Since we are not starting a new
          // operation the io_service will run out of work to do and the client will
          // exit.
        }

        connection_pool::ref().put(c);

        // Successfully established connection. Start operation to read the list
        // of stocks. The connection::async_read() function will automatically
        // decode the data that is read from the underlying socket.
        auto hander = std::bind(&tcp_client::handle_read, this, asio::placeholders::error);
        // _connection->async_read(stocks_, handler);
      }

      /// Handle completion of a read operation.
      void handle_read(const std::error_code& e) {
        if (e) return;
      }

      void handle_write() {}

      void close() {}

    private:

      connection_ptr _active_connectons;
    };

  } // rpc
} // atlasdb

#endif /* ATLASDB_RPC_CLIENT_H_ */
