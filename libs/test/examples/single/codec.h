#ifndef MUDUO_EXAMPLES_ASIO_CHAT_CODEC_H
#define MUDUO_EXAMPLES_ASIO_CHAT_CODEC_H

#include <muduo/base/Logging.h>
#include <muduo/net/Buffer.h>
#include <muduo/net/Endian.h>
#include <muduo/net/TcpConnection.h>

#include <boost/function.hpp>

#include <atlasdb/config.h>

class LengthHeaderCodec {
  public:

  typedef boost::function<void (const muduo::net::TcpConnectionPtr&,
                                const muduo::string& message,
                                muduo::Timestamp)> StringMessageCallback;

  explicit LengthHeaderCodec(const StringMessageCallback& cb) : messageCallback_(cb)
  {
    _buffer.ensureWritableBytes(reserved_buffer_size);
  }

  void onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf,
                 muduo::Timestamp receiveTime)
  {
    while (buf->readableBytes() >= kHeaderLen) {
      const void* data = buf->peek();
      uint64_t be64 = *static_cast<const uint64_t*>(data);
      const uint64_t len = muduo::net::sockets::networkToHost64(be64);

      if (len > reserved_buffer_size || len < 0) {
        LOG_ERROR << "Invalid length " << len;
        conn->shutdown();
      }
      else if (buf->readableBytes() >= len + kHeaderLen) {
        buf->retrieve(kHeaderLen);

        // Just for testing
        muduo::string message("Received");
        buf->retrieve(len);
        messageCallback_(conn, message, receiveTime);
      }
      else {
        break;
      }
    }
  }

  void send(muduo::net::TcpConnection* conn, const muduo::string& message) {
    using muduo::net::sockets::hostToNetwork64;
    muduo::net::Buffer buf;
    buf.append(message.data(), message.size());
    uint64_t len = hostToNetwork64(static_cast<uint64_t>(message.size()));

    buf.prepend(&len, sizeof len);
    conn->send(&buf);
  }

  void send(muduo::net::TcpConnection* conn, std::vector<char>& data) {
    using muduo::net::sockets::hostToNetwork64;
    // TODO: What's the difference between uint64_t and uint32_t in essential?
    uint64_t len = static_cast<uint64_t>(data.size() - prepended_gap_size);
    len = hostToNetwork64(len);

    _buffer.swap(data);
    _buffer.prepend(&len, sizeof len);
    conn->send(&_buffer);
  }

  void send(muduo::net::TcpConnection* conn, muduo::net::Buffer& buf) {
    uint64_t len = muduo::net::sockets::hostToNetwork64(static_cast<uint64_t>(buf.size()));
    buf.prepend(&len, sizeof len);
    conn->send(&buf);
  }

 private:
  StringMessageCallback messageCallback_;
  muduo::net::Buffer _buffer;

  const static size_t kHeaderLen = sizeof(uint64_t);
};

#endif  // MUDUO_EXAMPLES_ASIO_CHAT_CODEC_H
