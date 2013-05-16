/*
 * tcp_client_pool.h
 *
 *  Created on: May 1, 2013
 *      Author: vincent
 */

#ifndef ATLASDB_NET_TCP_CLIENT_POOL_H_
#define ATLASDB_NET_TCP_CLIENT_POOL_H_

#include <boost/ptr_container/ptr_map.hpp>

#include <atlas/singleton.h>
#include <atlasdb/net/tcp_client.h>

namespace atlasdb {
  namespace net {

    template<typename pool_tag>
    class tcp_client_pool : public atlas::singleton<tcp_client_pool<pool_tag>> {
    private:

      typedef  boost::ptr_map<std::string, tcp_client> tcp_client_container;

    public:

      void connect(const std::string& ip) {
        _clients.insert(ip, tcp_client());
      }

      void disconnect(const std::string& ip) {}

      void refresh(const std::string& ip) {}

    private:

      boost::ptr_multimap<std::string, tcp_client> _clients;
    };

  } // net
} // atlasdb

#endif /* ATLASDB_NET_TCP_CLIENT_POOL_H_ */
