/*
 * message.h
 *
 *  Created on: Apr 14, 2013
 *      Author: vincent
 */

#ifndef atlasdb_NET_MESSAGE_H_
#define atlasdb_NET_MESSAGE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace atlasdb {
  namespace net {

    class message {
    public:

      const static size_t header_length = 4;
      const static size_t max_body_length = 1024;

    public:

      message() : _body_length(0) {}

    public:

      const char* data() const { return _data; }
      char* data() { return _data; }

      const char* body() const { return _data + header_length; }
      char* body() { return _data + header_length; }

      size_t length() const { return header_length + _body_length; }
      size_t body_length() const { return _body_length; }

      void body_length(size_t length) {
        _body_length = length;
        if (_body_length > max_body_length) _body_length = max_body_length;
      }

      bool decode_header() {
        char header[header_length + 1] = "";

        std::strncat(header, _data, header_length);
        _body_length = std::atoi(header);
        if (_body_length > max_body_length) {
          _body_length = 0;
          return false;
        }

        return true;
      }

      void encode_header() {
        char header[header_length + 1] = "";
        std::sprintf(header, "%4d", _body_length);
        std::memcpy(_data, header, header_length);
      }

    private:

      char _data[header_length + max_body_length];
      size_t _body_length;
    };

  }
}
#endif /* MESSAGE_H_ */
