#include <set>
#include <cstdio>

#include "codec.h"

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/SocketsOps.h>
#include <muduo/net/TcpServer.h>

#include <boost/bind.hpp>

#include <atlasdb/dispatcher.h>

#include "initialization.h"

using namespace muduo;
using namespace muduo::net;

class ChatServer {
 public:
  ChatServer(EventLoop* loop, const InetAddress& listenAddr)
  : _env_open(false),
    loop_(loop),
    server_(loop, listenAddr, "ChatServer"),
    codec_(boost::bind(&ChatServer::onStringMessage, this, _1, _2, _3)) {
    server_.setConnectionCallback(
        boost::bind(&ChatServer::onConnection, this, _1));
    server_.setMessageCallback(
        boost::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
  }

  void start() {
    LOG_INFO << "Change current working directory";
    ::system("pwd");
    ::chdir("/home/vincent/workspace/morpheus/test/examples/single");
    ::system("pwd");

     _env.open("./env/");
     _env_open = true;

     LOG_INFO << "Environment is Ready";

    _dispatcher.reset(new atlasdb::dispatcher(_env));

//    for (int i = 0; i < 10; ++i) {
//      test_query();
//      ::sleep(3);
//    }

    server_.start();
  }

 private:

  void onConnection(const TcpConnectionPtr& conn) {
    LOG_INFO << conn->localAddress().toHostPort() << " -> "
        << conn->peerAddress().toHostPort() << " is "
        << (conn->connected() ? "UP" : "DOWN");

    if (conn->connected()) {
      connections_.insert(conn);
    }
    else {
      connections_.erase(conn);
    }
  }

  std::vector<char> do_query(const std::string& message) {
    std::vector<char> response = {'O', 'K', '!'};
    std::string info = "";

    try {
      unsigned long long xxstart = 0;
      unsigned long long xxend = 0;
      struct timeval tv;

      gettimeofday(&tv, nullptr);
      xxstart = tv.tv_sec * 1000 * 1000 + tv.tv_usec;

      _dispatcher->dispatch(message);
      response = _dispatcher->result();

      gettimeofday(&tv, nullptr);
      xxend = tv.tv_sec * 1000 * 1000 + tv.tv_usec;

      LOG_INFO << "Load time :" << ((xxend - xxstart) / 1000) << "\tLoaded size :" << response.size();
    }
    catch(std::bad_alloc& e) {
      // TODO
      info = e.what();
    }
    catch(std::exception& e) {
      // TODO
      info = e.what();
    }
    catch(std::string& e) {
      info = e;
    }
    catch(const char* ch) {
      info = ch;
    }
    catch(...) {
      info = "Unknown error";
    }

    if (!info.empty()) {
      std::copy(info.begin(), info.end(), std::back_inserter(response));
      LOG_INFO << "!!!!!!Exception : " << info;
    }

    return std::move(response);
  }

  void test_query() {
    std::ifstream ifs("load.json");
    std::stringstream message;
    message << ifs.rdbuf();

    std::vector<char> response = std::move(do_query(message.str()));

    LOG_INFO << "Result Size : " << response.size();
  }

  void onStringMessage(const TcpConnectionPtr&, const std::string& message, Timestamp) {
    LOG_INFO << "Message is coming ...";
    LOG_INFO << message;

    std::vector<char> response = std::move(do_query(message));

    for (auto it = connections_.begin(); it != connections_.end(); ++it) {
      LOG_INFO << "Response Size : [" << response.size() << "]";
      codec_.send(get_pointer(*it), response);
    }
  }

private:

  environment _env;
  bool _env_open;

  std::shared_ptr<atlasdb::dispatcher> _dispatcher;

  typedef std::set<TcpConnectionPtr> ConnectionList;
  EventLoop* loop_;
  TcpServer server_;
  LengthHeaderCodec codec_;
  ConnectionList connections_;
};

int main(int argc, char* argv[]) {
//  archiver archiver;
//  archiver.load();
//  archiver.test();
//
//return 0;

  if (argc > 1) {
    EventLoop loop;
    uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
    InetAddress serverAddr(port);
    ChatServer server(&loop, serverAddr);
    server.start();
    loop.loop();
  }
  else {
    printf("Usage: %s port\n", argv[0]);
  }
}
