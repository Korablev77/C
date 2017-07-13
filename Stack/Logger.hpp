#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <stdlib.h>
#include <stdio.h>

typedef enum {
	SEVERE,
	WARNING,
	INFO, 
	DEBUG
}LogLevel;


class StackLogger {
public:
	StackLogger();

	StackLogger(LogLevel lvl);

	static StackLogger getLogger(const char* name);

	void log(LogLevel lvl, const std::string msg) const;

	void debug(std::string msg) const;

	void info(std::string msg) const;

	void warning(std::string msg) const;

	void error(std::string msg) const;

	void setLevel(LogLevel level);

	~StackLogger();

private:
	LogLevel m_logLevel;
	FILE* m_fileToLog;
};

#endif

