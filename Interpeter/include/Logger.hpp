#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <stdio.h>
#include <time.h>

#include "Helpers.hpp"
#include <string>

typedef enum LogLevel {
	LOG,
	WARNING,
	ERROR
}LogLevel;


class Logger {
public:
	Logger(const char* name, const char* fileNameToDump = nullptr);

	void log(const char* msg, LogLevel logLvl = LOG);
	void log(const std::string &msg, LogLevel logLvl = LOG);

	void setLogLevel(LogLevel lvl);

	~Logger();

private:
	bool isLogPossible(LogLevel lvl) const;


	const char* m_name;
	FILE* m_fileToDump;
	LogLevel m_logLvl;

	const char* defaultFileToDump = "dump.txt";
};

extern Logger gLogger;


const char* logLevelToStr(LogLevel lvl);


#endif