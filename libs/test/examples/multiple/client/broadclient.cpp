/*
 * broadclient.cpp
 *
 *  Created on: 2012-2-27
 *      Author: jy
 */



#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>

/*********************************************************************
*filename: broadc-udpclient.c
*purpose: 基本编程步骤说明，演示了UDP编程的广播客户端编程步骤
*tidied by: zhoulifa(zhoulifa@163.com) 周立发(http://zhoulifa.bokee.com)
Linux爱好者 Linux知识传播者 SOHO族 开发者 最擅长C语言
*date time:2007-01-24 21:30:00
*Note: 任何人可以任意复制代码并运用这些文档，当然包括你的商业用途
* 但请遵循GPL
*Thanks to: Google.com
*Hope:希望越来越多的人贡献自己的力量，为科学技术发展出力
* 科技站在巨人的肩膀上进步更快！感谢有开源前辈的贡献！
*********************************************************************/
int main(int argc, char **argv)
{
  struct sockaddr_in s_addr;
  int sock;
  int addr_len;
  int len;
  char buff[128];
  int yes;

  /* 创建 socket */
  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(errno);
  } else
    printf("create socket.\n\r");

  /* 设置通讯方式对广播，即本程序发送的一个消息，网络上所有主机均可以收到 */
  yes = 1;
  setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));
  /* 唯一变化就是这一点了 */

  memset(&s_addr, 0, sizeof(s_addr));
  /* 设置对方地址和端口信息 */
  s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(5555);
    s_addr.sin_addr.s_addr = inet_addr("192.168.11.116");


  /* 发送UDP消息 */
  addr_len = sizeof(s_addr);
  strcpy(buff, "hello i'm here");
  len = sendto(sock, buff, strlen(buff), 0, (struct sockaddr *) &s_addr, sizeof (struct sockaddr_in));
  if (len < 0) {
  perror("sendto ");
  return -1;
}

  printf("send success.\n\r");
  return 0;
}

