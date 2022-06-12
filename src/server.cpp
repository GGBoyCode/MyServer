#include "server.h"

#include <stdlib.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <cstring>
#include <string>

extern study::Logger* logger;
extern bool loop;

namespace study {
	Server::Server(Protocol* p) : ptl(p) {}

	Server::~Server() {
		ptl->closeSocket(socket_fd);
		delete ptl;
	}

	void Server::startServer() {
		socket_fd = createSocket();
		setNonblock(socket_fd);
		bindSocket();
		listenSocket();

		epfd = epoll_create(256);

		if (addFdToEpoll(socket_fd, EPOLLIN) == -1) {
			exit(-1);
		}

		while (loop) {
			int event_number = epoll_wait(epfd, events, MAX_EVENTS, TIMEOUT);
			for (int i = 0; i < event_number; i++) {
				// 若事件为socket_fd引发，则创建新连接
				if (events[i].data.fd  == socket_fd) {
					int accept_fd = acceptSocket();
					if (accept_fd == -1) {
						char* s = "failed to accept\n";
						logger->log(s, strlen(s));
						continue;
					}
					setNonblock(accept_fd);
					addFdToEpoll(accept_fd, EPOLLET | EPOLLIN);
				}
				// 输入事件, 则响应客户端发送请求
				else if (events[i].events & EPOLLIN) {
					char recv_buff[MESSAGE_LEN], send_buff[MESSAGE_LEN];
					bzero(recv_buff, MESSAGE_LEN);
					int recvMessageSize = recvRequest(events[i].data.fd, recv_buff, MESSAGE_LEN);
					if (recvMessageSize <= 0) {
						close(events[i].data.fd);
						epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
					}

					logger->log(recv_buff, recvMessageSize);
					if (recvMessageSize == MESSAGE_LEN) {
						const char* err = "wornning: maybe have data...\n";
						logger->log(err, strlen(err));
					}
					int sendMessageSize = strlen(send_buff);
					setHTTPResponse(recv_buff, send_buff);
					logger->log(recv_buff, sendMessageSize);

					sendResponse(events[i].data.fd, send_buff, sendMessageSize);
					if (recvMessageSize < 0) {
						close(events[i].data.fd);
						epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
					}
				}
			}
		}
	}

	int Server::createSocket() {
		int socket_fd = ptl->createSocket();
		if (socket_fd == -1) {
			exit(-1);
		}

		char* s = "nomal: create a socket\n";
		logger->log(s, strlen(s));
		return socket_fd;
	}

	void Server::bindSocket() {
		ptl->bindSocket(socket_fd);
		if (socket_fd == -1) {
			exit(-1);
		}

		char* s = "nomal: bind port\n";
		logger->log(s, strlen(s));
	}

	void Server::listenSocket() {
		ptl->listenSocket(socket_fd);
		if (socket_fd == -1) {
			exit(-1);
		}

		char* s = "nomal: listen port\n";
		logger->log(s, strlen(s));
	}

	int Server::acceptSocket() {
		char* s = "nomal: accept...\n";
		logger->log(s, strlen(s));
		return ptl->acceptSocket(socket_fd);
	}
	
	int Server::recvRequest(int accept_fd, char* buff, int max) {
		return ptl->receive(accept_fd, buff, max);
	}

	int Server::sendResponse(int accept_fd, const char* buff, int len) {
		return ptl->sendResponse(accept_fd, buff, len);
	}

	void inline Server::setNonblock(int fd) {
		int flags = fcntl(fd, F_GETFL, 0);
		fcntl(fd, F_SETFL, flags);
	}

	int Server::addFdToEpoll(int fd, int events) {
		epoll_event ev;
		ev.events = events;
		ev.data.fd = fd;
		int flag = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
		// 未将socket_fd放入epoll中，则将错误打印到log中并退出程序
		if (flag == -1) {
			const char* err = ("error:" + std::to_string(errno) + strerror(errno) + "\n").c_str();
			logger->log(err, strlen(err));
		}

		return flag;
	}

	void Server::setHTTPResponse(char* recv_buff, char* send_buff) {
		// int recvLen = strlen(recv_buff);
	
		// for (int i = 0; i < recvLen; i++) {
		// 	if (recv_buff[i] == ' ') {
				
		// 	}
		// }
		bzero(send_buff, sizeof(send_buff));
		strcat(send_buff, "HTTP/1.1 200 OK\r\n");
		strcat(send_buff, "\r\n");
		strcat(send_buff, recv_buff);
		strcat(send_buff, "\n");
	}
}