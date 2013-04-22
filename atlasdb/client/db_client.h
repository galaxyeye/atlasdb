/*
 * db_client.h
 *
 *  Created on: Apr 14, 2013
 *      Author: vincent
 */

#ifndef atlasdb_CLIENT_DB_CLIENT_H_
#define atlasdb_CLIENT_DB_CLIENT_H_

#include <cstdlib>
#include <thread>
#include <system_error>
#include <functional>
#include <deque>

#include <boost/asio.hpp>

#include <atlasdb/net/message.h>

namespace atlasdb {
  namespace client {

    namespace asio = boost::asio;

    typedef std::deque<net::message> message_queue;

    class db_client {
    public:

      db_client(asio::io_service& io_service, asio::ip::tcp::resolver::iterator endpoint_iterator) :
        _io_service(io_service), _socket(io_service)
      {
        asio::async_connect(_socket, endpoint_iterator,
            std::bind(&db_client::handle_connect, this, asio::placeholders::error));
      }

      void write(const net::message& msg) {
        _io_service.post(std::bind(&db_client::do_write, this, msg));
      }

      void close() {
        _io_service.post(std::bind(&db_client::do_close, this));
      }

    private:

      void handle_connect(const std::error_code& error) {
        if (!error) {
          asio::async_read(_socket, asio::buffer(_read_msg.data(), net::message::header_length),
              std::bind(&db_client::handle_read_header, this, asio::placeholders::error));
        }
      }

      void handle_read_header(const std::error_code& error) {
        if (!error && _read_msg.decode_header()) {
          asio::async_read(_socket, asio::buffer(_read_msg.body(), _read_msg.body_length()),
              std::bind(&db_client::handle_read_body, this, asio::placeholders::error));
        }
        else {
          do_close();
        }
      }

      void handle_read_body(const std::error_code& error) {
        if (!error) {
          std::cout.write(_read_msg.body(), _read_msg.body_length());
          std::cout << "\n";
          asio::async_read(_socket, asio::buffer(_read_msg.data(), net::message::header_length),
              std::bind(&db_client::handle_read_header, this, asio::placeholders::error));
        }
        else {
          do_close();
        }
      }

      void do_write(net::message msg) {
        bool write_in_progress = !_write_msg.empty();
        _write_msg.push_back(msg);
        if (!write_in_progress) {
          asio::async_write(_socket, asio::buffer(_write_msg.front().data(), _write_msg.front().length()),
              std::bind(&db_client::handle_write, this, asio::placeholders::error));
        }
      }

      void handle_write(const std::error_code& error) {
        if (!error) {
          _write_msg.pop_front();
          if (!_write_msg.empty()) {
            asio::async_write(_socket, asio::buffer(_write_msg.front().data(), _write_msg.front().length()),
                std::bind(&db_client::handle_write, this, asio::placeholders::error));
          }
        }
        else {
          do_close();
        }
      }

      void do_close() { _socket.close(); }

    private:

      asio::io_service& _io_service;
      asio::ip::tcp::socket _socket;
      net::message _read_msg;
      message_queue _write_msg;
    };

    int main(int argc, char* argv[]) {
      try {
        if (argc != 3) {
          std::cerr << "Usage: db_client <host> <port>\n";
          return 1;
        }

        asio::io_service io_service;

        using namespace asio::ip;
        tcp::resolver resolver(io_service);
        tcp::resolver::query q(argv[1], argv[2]);
        auto iterator = resolver.resolve(q);

        db_client c(io_service, iterator);

        std::thread t(std::bind(&asio::io_service::run, &io_service));

        char line[net::message::max_body_length + 1];
        while (std::cin.getline(line, net::message::max_body_length + 1)) {
          net::message msg;
          msg.body_length(std::strlen(line));
          std::memcpy(msg.body(), line, msg.body_length());
          msg.encode_header();
          c.write(msg);
        }

        c.close();
        t.join();
      }
      catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
      }

      return 0;
    }

  } // client
} // atlasdb

#endif /* CLIENT_H_ */
