/*
 * client.test.cpp
 *
 *  Created on: 2012-2-23
 *      Author: jy
 */


#include <muduo/base/Atomic.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Thread.h>
#include <muduo/base/ThreadPool.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/TcpClient.h>
#include <muduo/net/Endian.h>

#include <boost/bind.hpp>

#include <utility>

#include <mcheck.h>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

using namespace muduo;
using namespace muduo::net;

class TimeClient : boost::noncopyable
{
public:
  TimeClient(EventLoop* loop, const InetAddress& listenAddr)
  : loop_(loop), client_(loop, listenAddr, "TimeClient")
  {
    client_.setConnectionCallback(boost::bind(&TimeClient::onConnection, this, _1));
    client_.setMessageCallback(boost::bind(&TimeClient::onMessage, this, _1, _2, _3));
    // client_.enableRetry();
  }

  void connect()
  {
    client_.connect();
  }

  private:
    void onConnection(const TcpConnectionPtr& conn)
    {
      LOG_INFO << conn->localAddress().toHostPort() << " -> "
         << conn->peerAddress().toHostPort() << " is "
         << (conn->connected() ? "UP" : "DOWN");

      if (!conn->connected())  // 如果连接断开，则终止主循环，退出程序
        loop_->quit();
      else {
        conn->send("test message", 13);
      }
    }



    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime)
    {
      LOG_INFO << "get data: " << buf->retrieveAsString();
      loop_->quit();
    }

   EventLoop* loop_;
   TcpClient client_;
};


int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid();
  //if (argc > 1)
  //{
    EventLoop loop;
    InetAddress serverAddr("127.0.0.1", 5554);
    //InetAddress serverAddr(argv[1], 2037);
    TimeClient timeClient(&loop, serverAddr);
    timeClient.connect();
    loop.loop();

  //}
 // else
  //{
  //  printf("Usage: %s host_ip\n", argv[0]);
 // }
}

