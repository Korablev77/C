#include "include/Logger.hpp"



Logger::Logger(const char* name, const char* fileNameToDump /* = nullptr*/) : m_name(name), m_logLvl(LOG) {
	if (fileNameToDump == nullptr) {
		fileNameToDump = defaultFileToDump;
	}
	m_fileToDump = fopen(fileNameToDump, "w");
	ASSERT((nullptr != m_fileToDump) && "Couldn't open file to log.");
	fprintf(m_fileToDump, "%s %s\n", "Start logging by", m_name);
	fflush(m_fileToDump);
}

void Logger::log(const char* msg, LogLevel logLvl /* = LOG*/) {
	ASSERT(nullptr != m_fileToDump);

	if (!isLogPossible(logLvl)) {
		return;
	}

	time_t rawTime;
	struct tm *timeInfo = nullptr;
	time(&rawTime);
	timeInfo = localtime(&rawTime);
	char timeString[10];
	strftime(timeString, sizeof(timeString), "%H:%M:%S", timeInfo);
	fprintf(m_fileToDump, "%s: %s ", logLevelToStr(logLvl), m_name);
	fprintf(m_fileToDump, "%s ", timeString);
	fprintf(m_fileToDump, "%s\n", msg);
	fflush (m_fileToDump);

	if (WARNING == logLvl || ERROR == logLvl) {
		fprintf(stdout, "%s: %s\n", logLevelToStr(logLvl), msg);
	}
}

void Logger::log(const std::string &msg, LogLevel logLvl /* = LOG*/) {
	log(msg.c_str(), logLvl);
}

void Logger::setLogLevel(LogLevel lvl) {
	m_logLvl = lvl;
}


bool Logger::isLogPossible(LogLevel lvl) const {
	if (m_logLvl == lvl) return true;
	if (lvl == ERROR) return true;
	if (lvl == WARNING && m_logLvl == LOG) return true;

	return false;
}


Logger::~Logger() {
	ASSERT((nullptr != m_fileToDump) && "Fclose argument is NULL.");
	ASSERT((EOF != fclose(m_fileToDump)) && "Fclose failed at closing file.");
}

Logger gLogger("Global Logger", "dump.txt");



const char* logLevelToStr(LogLevel lvl) {
	switch (lvl) {
		case LOG     : return "LOG";
		case WARNING : return "WARNING";
		case ERROR   : return "ERROR";
	}
	return nullptr;
}

