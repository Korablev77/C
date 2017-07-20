#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <stdio.h>
#include <string>
#include <stdexcept>



//$
//$
//$
//$
//$
//$


#ifdef EXCEPTION_MSG
#undef EXCEPTION_MSG
#endif

#define EXCEPTION_MSG(ExceptionType, msg) std::string(#ExceptionType) + " was thrown:\n-->" + std::string(msg) + \
	"\n-->At line: " + std::to_string(__LINE__) + "\n-->At function: " + std::string(__FUNCTION__) +\
	"\n-->At file: " + std::string(__FILE__) 

#ifdef THROW_EXCEPTION
#undef THROW_EXCEPTION
#endif

#define THROW_EXCEPTION(ExceptionType, msg, ...) throw ExceptionType(EXCEPTION_MSG(ExceptionType, msg), ## __VA_ARGS__)


class BaseException : public std::runtime_error {
public:
	explicit BaseException(std::string msg) : runtime_error(msg) {};
	explicit BaseException(const char* msg) : runtime_error(msg) {};

	virtual const char* what() const _NOEXCEPT;
	virtual ~BaseException()_NOEXCEPT = default;
};


class Token;

class InternalTranslationException : public BaseException {
public:
	InternalTranslationException(std::string msg, Token* token);

	virtual const char* what() const throw();
	virtual ~InternalTranslationException() throw() = default;
};

#include "Token.hpp"


void exceptionDispatcher();

#endif


