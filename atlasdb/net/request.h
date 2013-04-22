/**
 * request.h
 *
 *  Created on: 2012-5-11
 *      Author: vincent
 */

#ifndef NET_REQUEST_H_
#define NET_REQUEST_H_

#include <string>
#include <memory>

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <atlasdb/rpc/message.h>

namespace atlasdb {
  namespace net {

    using std::string;
    using boost::property_tree::basic_ptree;
    using boost::uuids::uuid;
    using boost::lexical_cast;

    // forward declaration
    // class query;

    /**
     * @see session
     * */
    class request : public std::enable_shared_from_this<request> {
    public:

      request(const char* message, size_t size) : _message(message, size) {
        boost::uuids::random_generator gen;

        _id = gen();

        _is_fresh_session = _message.session_id().empty();
        _is_fresh_query = _message.query_id().empty();

        _session_id = _is_fresh_session ? gen() : lexical_cast<uuid>(_message.session_id());
        _query_id = _is_fresh_query ? gen() : lexical_cast<uuid>(_message.query_id());

        _client_type = _message.client_type();
      }

      ~request() {
      }

    public:

      uuid id() const { return _id; }
      uuid query_id() const { return _query_id; }
      uuid session_id() const { return _session_id; }
      size_t tenement_id() const;
      size_t transaction_id() const;
      const std::vector<char>& buffer() const { return _buffer; }

      bool is_fresh_query() const { return _is_fresh_query; }
      bool is_fresh_session() const { return _is_fresh_session; }

      query_ptr query() const { return _query; }
      void query(query_ptr query) { _query = query; }
      session_ptr session() const { return _session; }
      void session(session_ptr session) { _session = session; }

      rpc::message get_message() const { return _message; }

    public:

      rpc::client_type client_type() const { return _client_type; }
      string client_ip() const { return _client_ip; }
      string connection_id() const { return _connection_id; }

      bool should_multicast() const {
        bool is_foreign_request = client_type() == rpc::client_type::foreign_client;
        rpc::query_method method = get_message().method();
        bool is_multicast_method = method == rpc::query_method::load
            || method == rpc::query_method::query
            || method == rpc::query_method::update
            || method == rpc::query_method::get_more;
        return is_foreign_request && is_multicast_method;
      }

    private:

      uuid _id;
      uuid _query_id;
      uuid _session_id;

      bool _is_fresh_query;
      bool _is_fresh_session;

      query_ptr _query;
      session_ptr _session;

      rpc::message _message;

      // hold the query cson buffer
      // const char* _query_buffer;
      // size_t _query_size;
      std::vector<char> _buffer;
      client_type _client_type;

      string _client_ip;
      string _connection_id;

      size_t _tenement_id;
      size_t _transaction_id;
    };

    bool operator==(const request& lhs, const request& rhs) const {
      return lhs.id() == rhs.id();
    }

    typedef std::shared_ptr<request> request_ptr;

    class foreign_request : public request {
    public:

      foreign_request(const char* message, size_t size) : request(message, size) {}
      ~foreign_request();

    public:

      bool should_multicast() const { return false; }
      auto next_leaf_query() const { return _sparated_query.find("1");}

    private:

      basic_ptree<string, std::pair<const char*, size_t>> _sparated_query;
    };

    class inner_request: public request {
    public:

      inner_request(const char* message, size_t size) : request(message, size) {}

    private:
    };
  } // db
} // atlasdb

#endif /* NET_REQUEST_H_ */
