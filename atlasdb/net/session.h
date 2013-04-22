/*
 * session.h
 *
 *  Created on: Mar 16, 2012
 *      Author: vincent
 */

#ifndef NET_SESSION_H_
#define NET_SESSION_H_

#include <string>
#include <vector>
#include <memory>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <atlasdb/net/query.h>

namespace atlasdb {
  namespace net {

    using boost::uuids::uuid;

    /*
     * Class Relationship:
     *
     *
     *          has many             has many
     * Session ------------> Query ------------> Request
     *         <------------       <------------
     *          belongs to           belongs to
     *
     * */
    class session: public std::enable_shared_from_this<session> {
    public:

      session(query_ptr query) {
        add_query(query);
      }
      explicit session(uuid id) : _id(id) {}

      uuid id() const { return _id; }
      const std::map<uuid, query_ptr>& queries() const { return _queries; }

    public:

      void add_query(query_ptr query) {
        _queries.insert(std::make_pair(query->id(), query));
      }

      void remove_query(query_ptr query) { _queries.erase(query->id()); }

    public:

      void merge_result(const char*, size_t);
      bool ready() const;

    private:
      uuid _id;
      std::map<uuid, query_ptr> _queries;
    };

    bool operator==(const session& lhs, const session& rhs) const {
      return lhs.id() == rhs.id();
    }

    typedef std::shared_ptr<session> session_ptr;


    class session_manager {
    public:

      void add(session_ptr session) {
        // _sessions.insert(std::make_pair(session->id(), request));
      }

      void remove(session_ptr session) {
        // TODO : check the semantic of map::erase
        _sessions.erase(session->id());
      }

      const std::map<size_t, session_ptr>& sessions() const {
        return _sessions;
      }

      session_ptr find(query_ptr query) {
        auto it = _sessions.find(query->session_id());
        if (it != _sessions.end()) return it->second;
        return session_ptr(nullptr);
      }

      session_ptr find(request_ptr request) {
        auto it = _sessions.find(request->session_id());
        if (it != _sessions.end()) return it->second;
        return session_ptr(nullptr);
      }

    private:

      std::map<uuid, session_ptr> _sessions;
    };

  } // namespace net
} // namespace atlasdb

#endif /* NET_SESSION_H_ */
