/*
 * context.h
 *
 *  Created on: Mar 30, 2012
 *      Author: vincent
 */

#ifndef atlasdb_DB_CONTEXT_H_
#define atlasdb_DB_CONTEXT_H_

#include <map>
#include <queue>
#include <vector>
#include <list>
#include <string>

#include <atlasdb/serialization/cson/BSONObj.h>

namespace atlasdb {
  namespace db {

    class request_context {
    public:
      void f();

    private:
      std::queue<cson::BSONObj> _queries;
      std::list<std::string> _multicast_ips;
      std::list<std::string> _multicast_channels;
      std::list<std::string> _return_nodes;

    };

    class query_session_context {

    };

    class transaction_context {
    private:
      std::map<int, query_session_context> sessions;
    };

    class context {
    public:

      context(unsigned int tenement_id);
      ~context();

    public:

      unsigned int tenement() const;

    private:
      /**
       * The tenement for the request, this field will affect the actual key
       */
      unsigned _tenement_id;

      /**
       * The query session of this query request
       * this query session can include several transactions, the
       */
      unsigned _session_id;

      unsigned _request_id;

      std::string _foreign_connection_name;
      std::string _inner_connection_name;

      std::string _foreign_client_ip; // reserved
      std::string _inner_client_ip; // reserved
    };
  } // db
} // atlasdb

#endif // atlasdb_DB_CONTEXT_H_
