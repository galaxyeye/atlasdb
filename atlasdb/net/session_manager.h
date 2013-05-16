/*
 * session_manager.h
 *
 *  Created on: May 1, 2013
 *      Author: vincent
 */

#ifndef ATLASDB_NET_SESSION_MANAGER_H_
#define ATLASDB_NET_SESSION_MANAGER_H_

#include <atlasdb/net/session.h>

namespace atlasdb {
  namespace net {

    class session_manager {
    public:

      void add(session_ptr session) {
      }

      void remove(const uuid& id) {
        _sessions.erase(id);
      }

      session_ptr find(const uuid& id) {
        auto s = _sessions.get(id);
        return s ? s.get() : nullptr;
      }

    private:

      atlas::concurrent_box<uuid, session_ptr> _sessions;
    };

  } // net
} // atlasdb

#endif /* ATLASDB_NET_SESSION_MANAGER_H_ */
