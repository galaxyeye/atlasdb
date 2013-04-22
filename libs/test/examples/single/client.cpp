#include <fstream>
#include <sstream>

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/TcpClient.h>

#include <boost/bind.hpp>

#include <atlasdb/dispatcher.h>

#include "initialization.h"
#include "codec.h"

using namespace muduo;
using namespace muduo::net;

unsigned long long xxstart = 0;
unsigned long long xxend = 0;
struct timeval tv;

class ChatClient {
 public:
  ChatClient(EventLoop* loop, const InetAddress& listenAddr)
    : loop_(loop),
      client_(loop, listenAddr, "ChatClient"),
      codec_(boost::bind(&ChatClient::onStringMessage, this, _1, _2, _3))
  {
    client_.setConnectionCallback(boost::bind(&ChatClient::onConnection, this, _1));
    client_.setMessageCallback(boost::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
    client_.enableRetry();
  }

  void connect() {
    client_.connect();
  }

  void disconnect() {
    // client_.disconnect();
  }

  void write(const string& cmd) {
    MutexLockGuard lock(mutex_);
    if (connection_) {
      codec_.send(get_pointer(connection_), cmd);
    }
  }

 private:

  void onConnection(const TcpConnectionPtr& conn) {
    LOG_INFO << conn->localAddress().toHostPort() << " -> "
        << conn->peerAddress().toHostPort() << " is "
        << (conn->connected() ? "UP" : "DOWN");

    MutexLockGuard lock(mutex_);
    if (conn->connected()) {
      connection_ = conn;
    }
    else {
      connection_.reset();
    }
  }

  void onStringMessage(const TcpConnectionPtr&, const string& result, Timestamp) {
    gettimeofday(&tv, nullptr);
    xxend = tv.tv_sec * 1000 * 1000 + tv.tv_usec;

    cout << "[Message]Time Elapsed:" << (xxend - xxstart)/1000 << endl;

    // printf("[%s]\n", cmd.c_str());
//    std::ofstream ofs("./output");
//    ofs.write(result.c_str(), result.size());
//    std::cout << "\n[Message]Query Result Received";
//    std::cout << "\n[Message]Result Size:" << result.size();

    std::cout << "\n\n\n<<< ";
    std::flush(cout);
  }

  EventLoop* loop_;
  TcpClient client_;
  LengthHeaderCodec codec_;
  MutexLock mutex_;
  TcpConnectionPtr connection_;
};

void print_help() {
  std::cout
      << "help: \n"
      << "1 test create table \n"
      << "2 test insert records \n"
      << "3 test load from database \n"
      << std::endl;
}

int main(int argc, char* argv[]) {
//  archiver ar;
//  // ar.prepare();
//  ar.load();
//
//  cout << ar.result_buf;

  if (argc > 2) {
    EventLoopThread loopThread;
    uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    InetAddress serverAddr(argv[1], port);

    ChatClient client(loopThread.startLoop(), serverAddr);
    client.connect();
    std::string line;

    std::cout << "<<< ";
    while (std::getline(std::cin, line)) {
      string cmd(line.c_str());

      try {
        if (cmd == "1") {
          std::ifstream ifs("create.json");
          std::stringstream message;
          message << ifs.rdbuf();

          client.write(message.str());
        }
        else if (cmd == "2") {
          std::ifstream ifs("insert.json");
          std::stringstream message;
          message << ifs.rdbuf();

          client.write(message.str());
        }
        else if (cmd == "3") {
          std::ifstream ifs("load.json");
          std::stringstream message;
          message << ifs.rdbuf();

          gettimeofday(&tv, nullptr);
          xxstart = tv.tv_sec*1000*1000+tv.tv_usec;

          client.write(message.str());
        }
      }
      catch(std::bad_alloc& e) {
        // TODO
          std::cout << e.what() << std::endl;
      }
      catch(std::exception& e) {
        // TODO
          std::cout << e.what() << std::endl;
      }
      catch(std::string& e) {
        std::cout << e << std::endl;
      }
      catch(const char* ch) {
        std::cout << ch << std::endl;
      }
      catch(...) {
        std::cout << "Unknown error" << std::endl;
      }

      print_help();
      std::cout << "<<< ";
      std::flush(cout);
    }

    client.disconnect();
  }
  else {
    print_help();
  }
}
