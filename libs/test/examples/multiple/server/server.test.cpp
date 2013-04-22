/*
 * server.cpp
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

#include <boost/bind.hpp>

#include <utility>

#include <mcheck.h>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class DBServer
{
 public:
  DBServer(EventLoop* loop, const InetAddress& listenAddr, int numThreads)
    : loop_(loop),
      server_(loop, listenAddr, "SudokuServer"),
      numThreads_(numThreads),
      startTime_(Timestamp::now())
  {
    server_.setConnectionCallback(
        boost::bind(&DBServer::onConnection, this, _1));
    server_.setMessageCallback(
        boost::bind(&DBServer::onMessage, this, _1, _2, _3));
  }

  void start()
  {
    LOG_INFO << "starting " << numThreads_ << " threads.";
    threadPool_.start(numThreads_);
    server_.start();
  }

 private:
  void onConnection(const TcpConnectionPtr& conn)
  {
    LOG_TRACE << conn->peerAddress().toHostPort() << " -> "
        << conn->localAddress().toHostPort() << " is "
        << (conn->connected() ? "UP" : "DOWN");
  }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp)
  {
    LOG_DEBUG << conn->name();
    //size_t len = buf->readableBytes();
    string ret(buf->retrieveAsString());
    //processRequest(conn, ret);
    threadPool_.run(boost::bind(&solve, conn, ret, ""));
  }
/*
  bool processRequest(const TcpConnectionPtr& conn, const string& request)
  {
    string id;
    string puzzle;
    bool goodRequest = true;

    string::const_iterator colon = find(request.begin(), request.end(), ':');
    if (colon != request.end())
    {
      id.assign(request.begin(), colon);
      puzzle.assign(colon+1, request.end());
    }
    else
    {
      puzzle = request;
    }

    if (puzzle.size() == implicit_cast<size_t>(kCells))
    {
      threadPool_.run(boost::bind(&solve, conn, puzzle, id));
    }
    else
    {
      goodRequest = false;
    }
    return goodRequest;
  }
*/
  static void solve(const TcpConnectionPtr& conn, const string& puzzle, const string& id)
  {
    LOG_DEBUG << conn->name();

    if (id.empty())
    {
      conn->send(puzzle+"\r\n");
    }
    else
    {
      conn->send(id+":"+puzzle+"\r\n");
    }
  }

  EventLoop* loop_;
  TcpServer server_;
  ThreadPool threadPool_;
  int numThreads_;
  Timestamp startTime_;
};

int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
  int numThreads = 0;
  if (argc > 1)
  {
    numThreads = atoi(argv[1]);
  }
  EventLoop loop;
  InetAddress listenAddr(2037);
  DBServer server(&loop, listenAddr, numThreads);

  server.start();

  loop.loop();
}



