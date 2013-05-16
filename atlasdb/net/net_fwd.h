/*
 * net_fwd.h
 *
 *  Created on: Apr 30, 2013
 *      Author: vincent
 */

#ifndef ATLAS_NET_FWD_H_
#define ATLAS_NET_FWD_H_

#include <memory>

namespace atlasdb {
  namespace net {

    class session;
    class request;
    class query;
    class response;
    class connection;

    typedef std::shared_ptr<session> session_ptr;
    typedef std::shared_ptr<request> request_ptr;
    typedef std::shared_ptr<query> query_ptr;
    typedef std::shared_ptr<response> response_ptr;
    typedef std::shared_ptr<connection> connection_ptr;

  } // net
} // atlas


#endif /* NET_FWD_H_ */
