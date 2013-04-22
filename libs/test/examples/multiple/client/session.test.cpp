/*
 * session.test.cpp
 *
 *  Created on: 2012-3-2
 *      Author: jy
 */
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <atlasdb/communication/appinterface/cachesession.h>

using atlasdb::communication::cache_session;

int main(int argc, char *argv[]) {

  std::ifstream ifs("test.json");
  std::stringstream msg;

  msg << ifs.rdbuf();

  cache_session session("127.0.0.1", 3370);
  session.begin_session();
  session.test_method(msg.str());
  session.end_session();

}


