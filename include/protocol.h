#pragma once

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <netinet/in.h>
#include <arpa/inet.h>
#include "logger.h"

#define PORT 8111
#define MAX_CONNECT 2048

namespace study {
	class Protocol {
	public:
		virtual ~Protocol() {};
	public:
		// 创建socket
		virtual int createSocket() = 0;
		// 绑定端口
		virtual void bindSocket(int socket_fd) = 0;
		// 监听端口
		virtual void listenSocket(int socket_fd) = 0;
		// 接受连接
		virtual int acceptSocket(int socket_fd) = 0;
		// 接收请求
		virtual int receive(int accept_fd, char* in_buff, const int len) = 0;
		// 发送相应
		virtual int sendResponse(int accept_fd, const char* in_buff, const int len) = 0;
		// 关闭连接
		virtual void closeSocket(int fd) = 0;
	protected:
		struct sockaddr_in localAddr, acceptAddr;
	};
} // study

#endif // __PROTOCOL_H__
