//
// tcp_server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ATLASDB_NET_SERVER_H_
#define ATLASDB_NET_SERVER_H_

#include <system_error>

#include <boost/asio/placeholders.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <atlasdb/net/net_fwd.h>
#include <atlasdb/net/connection_pool.h>

namespace atlasdb {
  namespace net {

    namespace asio = boost::asio;
    using asio::ip::tcp;

    class tcp_server {
    public:

      tcp_server(tcp_server&) = delete;
      tcp_server& operator=(tcp_server&) = delete;

    public:

      /// Construct the tcp_server to listen on the specified TCP address and port, and
      /// serve up files from the given directory.
      explicit tcp_server(const std::string& address, const std::string& port);

      /// Run the tcp_server's io_service loop.
      void run();

      /// Stop the tcp_server.
      void stop();

    private:

      /// Handle completion of an asynchronous accept operation.
      void handle_accept(const connection_ptr c, const std::error_code& e);

      /// Handle a request to stop the tcp_server.
      void handle_stop();

      /// The io_service used to perform asynchronous operations.
      asio::io_service _io_service;

      /// The signal_set is used to register for process termination notifications.
      asio::signal_set _signals;

      /// Acceptor used to listen for incoming connections.
      asio::ip::tcp::acceptor _acceptor;
    };

    tcp_server::tcp_server(const std::string& address, const std::string& port) :
        _signals(_io_service), _acceptor(_io_service)
    {
      // Register to handle the signals that indicate when the tcp_server should exit.
      // It is safe to register for the same signal multiple times in a program,
      // provided all registration for the specified signal is made through Asio.
      _signals.add(SIGINT);
      _signals.add(SIGTERM);
      _signals.add(SIGQUIT);

      _signals.async_wait(boost::bind(&tcp_server::handle_stop, this));

      // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
      tcp::resolver resolver(_io_service);
      tcp::resolver::query query(address, port);
      tcp::endpoint endpoint = *resolver.resolve(query);

      _acceptor.open(endpoint.protocol());
      _acceptor.set_option(tcp::acceptor::reuse_address(true));
      _acceptor.bind(endpoint);
      _acceptor.listen();

      connection_ptr c(new connection);
      auto handler = std::bind(&tcp_server::handle_accept, this, c, asio::placeholders::error);
      _acceptor.async_accept(c->socket(), handler);
    }

    void tcp_server::run() {
      // The io_service::run() call will block until all asynchronous operations
      // have finished. While the tcp_server is running, there is always at least one
      // asynchronous operation outstanding : the asynchronous accept call waiting
      // for new incoming connections.
      _io_service.run();
    }

    void tcp_server::stop() {
      // Post a call to the stop function so that tcp_server::stop() is safe to call
      // from any thread.
      _io_service.post(std::bind(&tcp_server::handle_stop, this));
    }

    void tcp_server::handle_accept(const connection_ptr c, const std::error_code& e) {
      if (e) return;

      connection_pool::ref().put(c);

      auto handler = std::bind(&tcp_server::handle_accept, this, c, asio::placeholders::error);
      _acceptor.async_accept(c->socket(), handler);
    }

    void tcp_server::handle_stop() {
      // The tcp_server is stopped by cancelling all outstanding asynchronous
      // operations. Once all operations have finished the io_service::run() call
      // will exit.
      _acceptor.close();
    }

  } // net
} // atlasdb

#endif // ATLASDB_NET_SERVER_HPP
