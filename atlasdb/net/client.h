/*
 * client.h
 *
 *  Created on: Apr 8, 2013
 *      Author: vincent
 */

#ifndef atlasdb_RPC_CLIENT_H_
#define atlasdb_RPC_CLIENT_H_

#include <string>

namespace atlasdb {
  namespace rpc {

    class client {
    public:

      virtual ~client() {}

      virtual void send(const char* message, size_t size) = 0;

      virtual void send(const std::string& message) = 0;

    };

    class p2p_client : public client {
    public:

      virtual ~p2p_client() {}

      virtual void send(const char* message, size_t size) {

      }

      virtual void send(const std::string& message) {

      }
    };

    class p2n_client : public client {
    public:

      virtual ~p2n_client() {}

      virtual void send(const char* message, size_t size) {

      }

      virtual void send(const std::string& message) {

      }
    };

    class multicast_client : public client {
    public:

      virtual ~multicast_client() {}

      virtual void send(const char* message, size_t size) {

      }

      virtual void send(const std::string& message) {

      }
    };

    class broadcast_client : public client {
    public:

      virtual ~broadcast_client() {}

      virtual void send(const char* message, size_t size) {
      }

      virtual void send(const std::string& message) {
      }
    };

  } // rpc
} // atlasdb

#endif /* MESSAGE_SENDER_H_ */
