/*
 * connection.h
 *
 *  Created on: Apr 14, 2013
 *      Author: vincent
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <tuple>
#include <system_error>

#include <boost/asio.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace atlasdb {
  namespace net {

    namespace asio = boost::asio;

/// The connection class provides serialization primitives on top of a socket.
    /**
     * Each message sent using this class consists of:
     * @li An 8-byte header containing the length of the serialized data in
     * hexadecimal.
     * @li The serialized data.
     */
    class connection {
    public:

      /// Constructor.
      connection(asio::io_service& io_service) : _socket(io_service) { }

      /// Get the underlying socket. Used for making a connection or for accepting
      /// an incoming connection.
      asio::ip::tcp::socket& socket() { return _socket; }

      /// Asynchronously write a data structure to the socket.
      template<typename T, typename... Handler>
      void async_write(const T& t, Handler handler...) {
        // Serialize the data first so we know how large it is.
        std::ostringstream archive_stream;
        boost::archive::text_oarchive archive(archive_stream);
        archive << t;
        _outbound_data = archive_stream.str();

        // Format the header.
        std::ostringstream header_stream;
        header_stream << std::setw(header_length) << std::hex << _outbound_data.size();
        if (!header_stream || header_stream.str().size() != header_length) {
          // Something went wrong, inform the caller.
          std::error_code error(asio::error::invalid_argument);
          _socket.get_io_service().post(boost::bind(handler, error));
          return;
        }
        _outhound_header = header_stream.str();

        // Write the serialized data to the socket. We use "gather-write" to send
        // both the header and the data in a single write operation.
        std::vector<asio::const_buffer> buffers;
        buffers.push_back(asio::buffer(_outhound_header));
        buffers.push_back(asio::buffer(_outbound_data));
        asio::async_write(_socket, buffers, handler...);
      }

      /// Asynchronously read a data structure from the socket.
      template<typename T, typename... Handler>
      void async_read(T& t, Handler handler...) {
        // Issue a read operation to read exactly the number of bytes in a header.
        void (connection::*f)(const std::error_code&, T&, std::tuple<Handler...>)
            = &connection::handle_read_header<T, Handler...>;

        auto h = boost::bind(f, this, asio::placeholders::error, boost::ref(t), std::make_tuple(handler...));
        asio::async_read(_socket, asio::buffer(_inbound_header), h);
      }

      /// Handle a completed read of a message header. The handler is passed using
      /// a tuple since boost::bind seems to have trouble binding a function object
      /// created using boost::bind as a parameter.
      template<typename T, typename... Handler>
      void handle_read_header(const std::error_code& e, T& t, std::tuple<Handler...> handler) {
        if (e) {
          std::get<0>(handler)(e);
        }
        else {
          // Determine the length of the serialized data.
          std::istringstream is(std::string(_inbound_header, header_length));
          std::size_t _inbound_datasize = 0;
          if (!(is >> std::hex >> _inbound_datasize)) {
            // Header doesn't seem to be valid. Inform the caller.
            std::error_code error(asio::error::invalid_argument);
            std::get<0>(handler)(error);
            return;
          }

          // Start an asynchronous call to receive the data.
          _inbound_data.resize(_inbound_datasize);
          void (connection::*f)(const std::error_code&, T&, std::tuple<Handler>)
          = &connection::handle_read_data<T, Handler>;
          asio::async_read(_socket, asio::buffer(_inbound_data),
              boost::bind(f, this, asio::placeholders::error, boost::ref(t), handler));
        }
      }

      /// Handle a completed read of message data.
      template<typename T, typename... Handler>
      void handle_read_data(const std::error_code& e, T& t, std::tuple<Handler...> handler) {
        if (e) {
          std::get<0>(handler)(e);
        }
        else {
          // Extract the data structure from the data just received.
          try {
            std::string archive_data(&_inbound_data[0], _inbound_data.size());  // copy 1
            std::istringstream archive_stream(archive_data);          // copy 2
            boost::archive::text_iarchive archive(archive_stream);
            archive >> t;
          }
          catch (std::exception& e) {
            // Unable to decode data.
            std::error_code error(asio::error::invalid_argument);
            std::get<0>(handler)(error);
            return;
          }

          // Inform caller that data has been received ok.
          std::get<0>(handler)(e);
        }
      }

    private:

      /// The underlying socket.
      asio::ip::tcp::socket _socket;

      /// The size of a fixed length header.
      enum { header_length = 8 };

      /// Holds an outbound header.
      std::string _outhound_header;

      /// Holds the outbound data.
      std::string _outbound_data;

      /// Holds an inbound header.
      char _inbound_header[header_length];

      /// Holds the inbound data.
      std::vector<char> _inbound_data;
    };

    typedef std::shared_ptr<connection> connection_ptr;

  }
}

#endif /* CONNECTION_H_ */
