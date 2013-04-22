/*
 * db_server.h
 *
 *  Created on: Apr 14, 2013
 *      Author: vincent
 */

#ifndef atlasdb_SERVER_DB_SERVER_H_
#define atlasdb_SERVER_DB_SERVER_H_

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <list>
#include <set>
#include <memory>
#include <functional>

#include <boost/asio.hpp>

#include <atlasdb/net/message.h>

namespace atlasdb {
  namespace net {

    namespace asio = boost::asio;
    using boost::asio::ip::tcp;

    typedef std::deque<message> message_queue;

    class db_session: public std::enable_shared_from_this<db_session> {
    public:

      db_session(asio::io_service& io_service) : _socket(io_service) {}

      tcp::socket& socket() { return _socket; }

      void start() {
        auto handler = std::bind(&db_session::handle_read_header, shared_from_this(), asio::placeholders::error);
        asio::async_read(_socket, asio::buffer(_read_msg.data(), message::header_length), handler);
      }

      void deliver(const message& msg) {
        bool write_in_progress = !_write_msgs.empty();
        _write_msgs.push_back(msg);

        if (!write_in_progress) {
          auto b = asio::buffer(_write_msgs.front().data(), _write_msgs.front().length());
          auto handler = std::bind(&db_session::handle_write, shared_from_this(), asio::placeholders::error);
          asio::async_write(_socket, b, handler);
        }
      }

      void handle_read_header(const std::error_code& error) {
        if (!error && _read_msg.decode_header()) {
          auto b = asio::buffer(_read_msg.body(), _read_msg.body_length());
          auto handler = std::bind(&db_session::handle_read_body, shared_from_this(), asio::placeholders::error);

          asio::async_read(_socket, b, handler);
        }
      }

      void handle_read_body(const std::error_code& error) {
        if (error) return;



        asio::async_read(_socket, asio::buffer(_read_msg.data(), message::header_length),
            std::bind(&db_session::handle_read_header, shared_from_this(), asio::placeholders::error));
      }

      void handle_write(const std::error_code& error) {
        if (error) return;

        _write_msgs.pop_front();
        if (!_write_msgs.empty()) {
          asio::async_write(_socket, asio::buffer(_write_msgs.front().data(), _write_msgs.front().length()),
              std::bind(&db_session::handle_write, shared_from_this(), asio::placeholders::error));
        }
      }

    private:

      tcp::socket _socket;

      message _read_msg;
      message_queue _write_msgs;
    };

    typedef std::shared_ptr<db_session> db_session_ptr;

    class db_server {
    public:

      db_server(asio::io_service& io_service, const tcp::endpoint& endpoint) :
          io_service_(io_service), acceptor_(io_service, endpoint) {
        db_session_ptr new_session(new db_session(io_service_));
        acceptor_.async_accept(new_session->socket(),
            std::bind(&db_server::handle_accept, this, new_session, asio::placeholders::error));
      }

      void handle_accept(db_session_ptr session, const std::error_code& error) {
        if (error) return;

        session->start();
        db_session_ptr new_session(new db_session(io_service_));

        auto handler = std::bind(&db_server::handle_accept, this, new_session, asio::placeholders::error);
        acceptor_.async_accept(new_session->socket(), handler);
      }

    private:

      asio::io_service& io_service_;
      tcp::acceptor acceptor_;
    };

  } // net
} // atlasdb

#endif /* DB_SERVER_H_ */
