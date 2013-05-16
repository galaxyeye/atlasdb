/*
 * transmitter.h
 *
 *  Created on: Apr 30, 2013
 *      Author: vincent
 */

#ifndef ATLASDB_NET_TRANSMITTER_H_
#define ATLASDB_NET_TRANSMITTER_H_

#include <string>

#include <boost/asio/ip/udp.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace atlasdb {
  namespace net {

    using asio::ip::udp;
    using asio::ip::tcp;

    class transmitter {
    public:

      virtual ~transmitter() {}

      virtual void send(const char* message, size_t size)  = 0;

      virtual void send(const std::string& message) {
        send(message.c_str(), message.size());
      }
    };

    class p2p_transmitter : public transmitter {
    public:

      virtual ~p2p_transmitter() {}

      virtual void send(const char* message, size_t size) {

      }

    private:

      tcp::endpoint _endpoint;
    };

    class p2n_transmitter : public transmitter {
    public:

      virtual ~p2n_transmitter() {}

      virtual void send(const char* message, size_t size) {

      }
    };

    class mcast_transmitter : public transmitter {
    public:

      virtual ~mcast_transmitter() {}

      virtual void send(const char* message, size_t size) {

      }
    };

    class bcast_transmitter : public transmitter {
    public:

      virtual ~bcast_transmitter() {}

      virtual void send(const char* message, size_t size) {

      }
    };

  } // net
} // atlasdb

#endif /* ATLASDB_NET_TRANSMITTER_H_ */
