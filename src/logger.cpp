#include <cstring>

#include "logger.h"

namespace study {
	Logger* Logger::logger = nullptr;

	Logger* Logger::getInstance() {
		if (logger == nullptr) {
			logger = new Logger();
		}

		return logger;
	}

	void Logger::destroy() {
		delete this;
	}

	Logger::Logger() {
		bzero(buff, MAX_BUFFSIZE);
		buffLen = 0;
		fp = fopen("server.log", "a");
	}

	Logger::~Logger() {
		if (buffLen > 0) {
			flush();
		}
		fclose(fp);
	}

	void Logger::flush() {
		fputs(buff, fp);
		bzero(buff, MAX_BUFFSIZE);
		buffLen = 0;
	}


	void Logger::log(const char* str, int len) {
		if (len + buffLen >= MAX_BUFFSIZE) {
			flush();
		}

		for (int i = 0; i < len; i++) {
			buff[buffLen] = str[i];
			buffLen++;
		}
	}
} // namespace study