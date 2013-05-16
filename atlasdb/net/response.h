/*
 * response.h
 *
 *  Created on: 2012-5-11
 *      Author: vincent
 */

#ifndef ATLAS_NET_RESPONSE_H_
#define ATLAS_NET_RESPONSE_H_

#include <boost/asio/streambuf.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <atlasdb/net/net_fwd.h>
#include <atlasdb/net/message.h>

namespace atlasdb {
  namespace net {

    namespace asio = boost::asio;
    using asio::ip::tcp;

    class response {
    public:

      response(const tcp::endpoint& endpoint) : _endpoint(endpoint) {}

      const tcp::endpoint& endpoint() const { return _endpoint; }

    private:

      tcp::endpoint _endpoint;
      asio::streambuf _message;
    };

  } // net
} // atlasdb

#endif /* ATLAS_NET_RESPONSE_H_ */
