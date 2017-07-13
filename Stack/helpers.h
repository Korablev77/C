#ifndef HELPERS_H
#define HELPERS_H

#define DEBUG 

#ifdef DEBUG 

#include <exception>
#include <string>
#include <stdio.h>
#include "Logger.hpp"

StackLogger logger;

#define ASSERT(cond) do{ \
  if (!(cond)) { \
    std::cerr << "EXPRESSION: " << #cond << "LINE: " << __LINE__ \
    << "FUNCTION: " << __FUNCTION__ << "FILE: " << __FILE__; \
  exit(1); \
  } \
}while(0)


class AssertFailureException : public std::runtime_error{
public:
  explicit AssertFailureException(const std::string& msg) : std::runtime_error(msg.c_str()) {};
  explicit AssertFailureException(const char* msg) : std::runtime_error(msg) {}
private:
};


#define ASSERT_SOFT(cond) do{ \
  if (!(cond)) { \
  	std::ostringstream oss; \
  	oss << "SOFT ASSERTATION FAILED\n"; \
  	oss << "EXPRESSION: " << #cond << " LINE: " << __LINE__ \
    << " FUNCTION: " << __FUNCTION__ << " FILE: " << __FILE__ << std::endl; \
  throw AssertFailureException(oss.str()); \
  }\
}while(0)



#define WARNING(msg) do{ \
  fprintf(stderr, "! %s ! \n", msg); \
}while(0)

#define STACK_OK() do{ \
	ASSERT_SOFT(isOk() != 0); \
}




class TraceLogger {
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


/*int f() {...}

#define f()   ( DoTrace (__FUNCTION__, "f"), f() ) 

int i = f();

int main()
{
	int i = f();
}*/


#else 
#define ASSERT(cond) ((void)0)
#define ASSERT_SOFT(cond) ((void)0)
#define WARNING(msg) ((void)0)
#define LOG_TRACE() ((void)0)

#define STACK_OK() do { \
	
}

#endif

#endif

