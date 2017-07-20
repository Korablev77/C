#ifndef LEXER_HPP
#define LEXER_HPP


#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <vector>

#include "Token.hpp"
#include "TokenHolder.hpp"
#include "Constants.hpp"
#include "Logger.hpp"
#include "Helpers.hpp"


/*#ifdef DEBUG

#define MEM_CHECK_BEFORE 66
#define MEM_CHECK_AFTER  77
#define FILL_CHAR 0

void* operator new(size_t nbytes) throw(std::bad_alloc) {
	char *pBuf = ::new char[nbytes+4];
	memset(pBuf+2, FILL_CHAR, nbytes);
	*(unsigned*)pBuf = MEM_CHECK_BEFORE;
	*(unsigned*)(pBuf+nbytes+2) = MEM_CHECK_AFTER;

	return (void*)(pBuf + 2);
}

void operator delete(void* pBuf, size_t nbytes) {
	if (*(unsigned*)((char*)pBuf-2) != MEM_CHECK_BEFORE || *(unsigned*)((char*)pBuf+nbytes) != MEM_CHECK_AFTER) {
		fprintf(stderr, "%s\n", "Delete: Memory was overitten!");
	}
	::delete ((char*)pBuf-2);
}

#endif*/


//TODO: Заменить на конечный автомат(см Хантера)
class Lexer {
public:
	Lexer();
	TokenHolder* tokenize(const char* expr);
	~Lexer() = default;
private:
	Token* recognizeCurrentToken();

	char* m_currentSymbol;
	char* m_nextLexem;
};


#endif






