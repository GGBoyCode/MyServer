#pragma once
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#define MAX_BUFFSIZE 1024

namespace study {
	class Logger {
	private:
		Logger();
		~Logger();
	public:
		static Logger* getInstance();
		void destroy();
		void flush();
		void log(const char* str, int len);
	public:
		static Logger* logger;
	private:
		char buff[MAX_BUFFSIZE];
		int buffLen;
		FILE* fp;
	};
} // namespace study
#endif