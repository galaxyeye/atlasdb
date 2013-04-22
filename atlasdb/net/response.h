/*
 * response.h
 *
 *  Created on: 2012-5-11
 *      Author: vincent
 */

#ifndef RESPONSE_H_
#define RESPONSE_H_

#include <string>

namespace atlasdb {
  namespace net {

    using std::string;
    using boost::property_tree::basic_ptree;

    // forward declaration
    class request;
    class result_buffer_ptr;

    class response {
    public:

      response() {}

      ~response() {}

      request request() const {
        return _request;
      }

      result_buffer_ptr result_buffer() const {
        return _result_buffer;
      }

    private:
      // hold the query cson buffer
      const char* _query_buffer;
      size_t _query_size;

      request _request;

      result_buffer_ptr _result_buffer;

      bool _is_inner_request;

      string _connection_id;
      string _client_ip;

      size_t _tenement_id;
      size_t _transaction_id;
    };
  } // db
} // atlasdb

#endif /* RESPONSE_H_ */
