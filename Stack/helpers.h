#ifndef HELPERS_H
#define HELPERS_H

#define DEBUG 

#ifdef DEBUG 

#define ASSERT(cond) do{ \
  if (!(cond)) {\
    std::cerr << "EXPRESSION: " << #cond << "LINE: " << __LINE__ \
    << "FUNCTION: " << __FUNCTION__ << "FILE: " << __FILE__; \
  exit(1);\
  }\
}while(0)



#define WARNING(msg) do{ \
	std::cerr << "WARNING: " << msg; \
}while(0)

#else 
#define ASSERT(cond) ((void)0)
#define WARNING(msd) ((void)0)
#endif

#endif