#include "Logger.hpp"

StackLogger::StackLogger() :  
	m_logLevel(SEVERE), m_fileToLog(fopen("log.txt", "w")) {}

StackLogger::StackLogger(LogLevel lvl) :
	 m_logLevel(lvl), m_fileToLog(fopen("log.txt", "w")) {} 

void StackLogger::log(LogLevel lvl, const std::string msg) const {
	fprintf(m_fileToLog, "%s\n", msg.c_str());
	fflush(m_fileToLog);
}

void StackLogger::debug(std::string msg) const {
	if (m_logLevel != DEBUG) return;
	fprintf(m_fileToLog, "$DEBUG$ :  %s\n", msg.c_str()); 
}

void StackLogger::info(std::string msg) const {
	if (m_logLevel != INFO) return;
	fprintf(m_fileToLog, "INFO : %s\n", msg.c_str());
}

void StackLogger::warning(std::string msg) const {
	if (m_logLevel == SEVERE) return;
	fprintf(m_fileToLog, "!WARNING! : %s\n", msg.c_str());
}

void StackLogger::error(std::string msg) const {
	fprintf(m_fileToLog,"!!!ERROR!!! :" "%s\n", msg.c_str());
}

void StackLogger::setLevel(LogLevel level) {
	m_logLevel = level;
}

StackLogger::~StackLogger() {
	fclose(m_fileToLog);
}

