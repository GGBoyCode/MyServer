#include "tcp.h"

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <string>
#include <sys/errno.h>

extern study::Logger* logger;

namespace study {
	Tcp::Tcp() {
		bzero(&localAddr, sizeof(localAddr));
		bzero(&acceptAddr, sizeof(acceptAddr));
	}

	Tcp::~Tcp() {}

	int Tcp::createSocket() {
		int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (socket_fd == -1) {
			err();
		}
		
		return socket_fd;
	}

	void Tcp::bindSocket(int socket_fd) {
		bzero(&localAddr, sizeof(localAddr));
		localAddr.sin_family = AF_INET;
		localAddr.sin_port = htons(PORT);
		localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		int res = bind(socket_fd, (sockaddr*)&localAddr, sizeof(localAddr));
		if (res == -1) {
			err();
		}
	}

	void Tcp::listenSocket(int socket_fd) {
		int res = listen(socket_fd, MAX_CONNECT);
		if (res == -1) {
			err();
		}
	}

	int Tcp::acceptSocket(int socket_fd) {
		int accept_fd = -1;
		socklen_t addrSize = sizeof(acceptAddr);
		accept_fd = accept(socket_fd, (sockaddr*)&acceptAddr, &addrSize);
		if (accept_fd == -1) {
			err();
		}
		return accept_fd;
	}

	int Tcp::receive(int accept_fd, char* in_buff, const int len) {
		int res = recv(accept_fd, (void*)in_buff, len, 0);
		return res;
	}

	int Tcp::sendResponse(int accept_fd, const char* in_buff, const int len) {
		int res = send(accept_fd, (void*)in_buff, strlen(in_buff), 0);
		return res;
	}

	void Tcp::closeSocket(int fd) {
		int res = close(fd);
		if (res == -1) {
			err();
		}
	}

	void inline Tcp::err() {
		const char* err = ("error:" + std::to_string(errno) + strerror(errno) + "\n").c_str();
		logger->log(err, strlen(err));
		exit(-1);
	}
} // namespace study