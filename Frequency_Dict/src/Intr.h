#pragma once

#include <cstring>

#if defined(__GNUC__) && __GNUC__ >= 4
	#define my_memcpy(d,s,l) __builtin_memcpy((d),(s),(l))
	#define my_memcmp(d,s,l) __builtin_memcmp((d),(s),(l))
#else
	#define my_memcpy(d,s,l) std::memcpy((d),(s),(l))
	#define my_memcmp(d,s,l) std::memmove((d),(s),(l))
#endif
