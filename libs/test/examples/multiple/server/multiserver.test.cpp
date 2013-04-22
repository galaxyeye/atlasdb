/*
 * multiserver.test.cpp
 *
 *  Created on: 2012-2-28
 *      Author: jy
 */
#include <atlasdb/communication/catalog/catalogmanager.h>
#include <atlasdb/communication/session/sessionmanager.h>
#include <atlasdb/communication/server/broadcastserver.h>
#include <atlasdb/communication/server/tcpserver.h>

int main(int argc, char* argv[]) {
  LOG_INFO << "pid = " << getpid() << ", tid = " << muduo::CurrentThread::tid();

  int numThreads = 0;
  if (argc > 1) {
    numThreads = atoi(argv[1]);
  }

  muduo::net::EventLoop loop;
  atlasdb::communication::catalog_manager catalogmanager;
  atlasdb::communication::session_manager sessionmanager;

  muduo::net::InetAddress listenAddr(3370);
  atlasdb::communication::tcp_server server(&loop, listenAddr, numThreads, &catalogmanager, &sessionmanager);
  server.start();

  using atlasdb::communication::REPLYPORT;
  muduo::net::InetAddress replyAddr(REPLYPORT);
  atlasdb::communication::reply_server reply(&loop, replyAddr, &sessionmanager);
  reply.start();

  atlasdb::communication::broadcast_server broadcastserver;
  broadcastserver.start_server_async(&loop);

  loop.loop();
}
