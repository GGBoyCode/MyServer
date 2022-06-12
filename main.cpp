#include <cstdio>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>

#include "server.h"
#include "tcp.h"
#include "logger.h"

study::Logger* logger = study::Logger::getInstance();
bool loop = true;

void handler(int sig) {
	logger->destroy();
	loop = false;
}

int main()
{
	if (daemon(0, 0) == -1) {
			exit(-1);
	}

	struct sigaction act;

	act.sa_handler = handler;
	sigfillset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGINT, &act, nullptr);
    study::Protocol* pro = new study::Tcp();
    study::Server server(pro);
    server.startServer();

    return 0;
}