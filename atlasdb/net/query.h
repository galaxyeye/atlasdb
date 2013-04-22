/*
 * query.h
 *
 *  Created on: 2012-5-14
 *      Author: vincent
 */

#ifndef QUERY_H_
#define QUERY_H_

#include <string>
#include <map>
#include <memory>

#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <atlasdb/query/engine.h>
#include <atlasdb/net/request.h>

namespace atlasdb {
  namespace net {

    namespace q = atlasdb::query;

    using boost::uuids::uuid;

    typedef std::vector<char> result_buffer;
    typedef std::shared_ptr<result_buffer> result_buffer_ptr;

    // forward declaration
    // class session_ptr;

    /**
     * @see session
     * */
    class query : public std::enable_shared_from_this<query> {
    public:

      query(request_ptr request) {
        add_request(request);
      }

      uuid id() const { return _id; }
      uuid session_id() const { return _session_id; }
      const std::map<uuid, request_ptr>& requests() const { return _requests; }

      session_ptr session() const { return _session; }
      void session(session_ptr session) { _session = session; }

    public:

      void add_request(request_ptr request) {
        _requests.insert(std::make_pair(request->id(), request));
      }

      void remove_request(request_ptr request) { _requests.erase(request->id()); }

    private:

      std::vector<char> _query_buffer;

      uuid _id;
      uuid _session_id;

      session_ptr _session;

      std::map<uuid, request_ptr> _requests;
    };

    typedef std::shared_ptr<query> query_ptr;

    bool operator==(const query& lhs, const query& rhs) const {
      return lhs.id() == rhs.id();
    }
  } // net
} // atlasdb

#endif /* QUERY_H_ */
