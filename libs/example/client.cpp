/*
 * client.cpp
 *
 *  Created on: May 1, 2013
 *      Author: vincent
 */


#include <atlasdb/net/tcp_client.h>

using namespace atlasdb;
namespace asio = boost::asio;
using asio::ip::tcp;

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

    net::tcp_client client(io_service, iterator);

    std::thread t([=]() { io_service.run(); });

    char line[net::message::max_body_length + 1];
    while (std::cin.getline(line, net::message::max_body_length + 1)) {
      net::message msg;
      msg.body_length(std::strlen(line));
      std::memcpy(msg.body(), line, msg.body_length());
      msg.encode_header();
      client.write(msg);
    }

    client.close();
    t.join();
  }
  catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
