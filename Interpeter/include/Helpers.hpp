#ifndef HELPERS_H
#define HELPERS_H

#include <exception>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG

#ifdef DEBUG

#define ASSERT(cond) do{ \
  if (!(cond)) { \
     fprintf(stderr, "%s\n", "=== ASSERTATION HAS OCCURED ==="); \
     fprintf(stderr, "%s %s\n", "EXPRESSION: ", #cond); \
     fprintf(stderr, "%s %d\n", "LINE: " ,__LINE__); \
     fprintf(stderr, "%s %s\n","FUNCTION: ", __FUNCTION__); \
     fprintf(stderr, "%s %s\n", "FILE: ", __FILE__); \
  exit(1); \
  } \
}while(0)



/*class TraceLogger {
public:
	TraceLogger(const char *fileName, const char *funcName, int line) : m_fileName(fileName), m_funcName(funcName) {
		fprintf(stdout, "%*s Entering %s() - (%s : %d)\n", indent*2, "", m_funcName, m_fileName, line);
		//fflush(stdout);
		//m_indent.append("  ");
		indent += 2;
	}
	~TraceLogger() {
		fprintf(stdout, "%*s Leaving %s()  - (%s)\n", indent*2, "", m_funcName, m_fileName);
		//fflush(stdout);
		//m_indent.resize(m_indent.length() - 2);
		indent -= 2;
	}

private:
	const char *m_fileName;
	const char *m_funcName;

	static int indent; 

	static std::string m_indent;
};

std::string TraceLogger::m_indent = "  ";
int TraceLogger::indent = 2;

#define LOG_TRACE() TraceLogger(__FILE__, __FUNCTION__, __LINE__)

*/
#else 
#define ASSERT(cond) ((void)0)
//#define LOG_TRACE() ((void)0)


#endif

#endif