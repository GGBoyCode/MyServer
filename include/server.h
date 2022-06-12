#pragma once

#ifndef __SERVER_H__
#define __SERVER_H__

#include "protocol.h"

#include <sys/epoll.h>

#define TIMEOUT 500
#define MAX_EVENTS 20
#define MESSAGE_LEN 1024

namespace study {

	class Server {
	public:
		Server(Protocol* p);
		~Server();
	public:
		// 启动服务器
		void startServer();

		// 创建连接
		int createSocket();

		// 绑定连接
		void bindSocket();

		// 监听连接
		void listenSocket();

		// 接受连接
		int acceptSocket();

		// 接收请求
		int recvRequest(int accept_fd, char* buff, int max);

		// 回复响应
		int sendResponse(int accept_fd, const char* buff, int len);

		// 设置非阻塞IO
		void inline setNonblock(int fd);

		// 将文件标识符加入epoll中
		int addFdToEpoll(int fd, int events);

		// 设置HTTP响应报文
		void setHTTPResponse(char* recv_buff, char* send_buff);
	private:
		Protocol* ptl;
		int epfd, socket_fd;
		epoll_event events[MAX_EVENTS];
	};

} // namespace study

#endif

