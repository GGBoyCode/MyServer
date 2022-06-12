#pragma once

#ifndef __TCP_H__
#define __TCP_H__
#include "protocol.h"

namespace study {
	class Tcp : public Protocol {
	public:
		Tcp();
		~Tcp();
	public:
		virtual int createSocket();
		virtual void bindSocket(int socket_fd);
		virtual void listenSocket(int socket_fd);
		virtual int acceptSocket(int socket_fd);
		virtual int receive(int accept_fd, char* in_buff, const int len);
		virtual int sendResponse(int accept_fd, const char* in_buff, const int len);
		virtual void closeSocket(int fd);
	private:
		// 输出错误日志
		void inline err();

	};
} // namespace study


#endif
