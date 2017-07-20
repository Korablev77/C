#include "include/Lexer.hpp"
#define DEBUG

Lexer::Lexer() : m_currentSymbol(NULL), m_nextLexem(NULL) {}


TokenHolder* Lexer::tokenize(const char* expr) {

	ASSERT((nullptr != expr) && "Lexer has got a null instead of expression.");
	gLogger.log("=== Lexer starts parsing ===");
	gLogger.log(std::string("Processing expression: ") + expr);
	m_currentSymbol = (char*)expr;
	m_nextLexem     = (char*)expr;

	std::vector<Token*> tokens;
	tokens.reserve(LINE_LIMIT);

	while (*m_currentSymbol) {
		Token* currentToken = recognizeCurrentToken();
		if (nullptr == currentToken) continue;
		gLogger.log(std::string("Recognized token: ") + currentToken->toString());
		tokens.push_back(currentToken);
	}
	tokens.push_back(TokenFabric::createToken<SymbolMnemonic>(SYMBOL, EOL));
	m_currentSymbol = nullptr;
	m_nextLexem     = nullptr;
	gLogger.log("===Lexer ends parsing===");

	return new TokenHolder(tokens);
}

static bool isDigit(const char c) {
	if ('0' <= c && c <= '9') return true;
	return false;
}

static bool isServiceSymbol(const char c) {
	for (auto s : SERVICE_SYMBOLS) {
		if (s == c) return true;
	}
	return false;
}

static bool isLetter(const char c) {
	if ('a' <= c && c <= 'z') return true;
	return false;
}

static bool isKeyword(const char* s) {
	for (auto word : KEYWORDS) {
		if (strcmp(word, s) == 0) return true;
	}
	return false;
}


Token* Lexer::recognizeCurrentToken() {
	Token *token = nullptr;
	while(' ' == *m_currentSymbol) {
		gLogger.log("Skipping space...");
		m_currentSymbol++; 
	} 

	gLogger.log(std::string("Current symbol is ") + *m_currentSymbol);
	m_nextLexem = m_currentSymbol;

	if (isServiceSymbol(*m_currentSymbol)) {

		gLogger.log(std::string("Lexer is recognizing service symbol: ") + *m_currentSymbol);
		token = TokenFabric::createToken<SymbolMnemonic>(SYMBOL, recognizeSymbolMnemonic(*m_currentSymbol++));

	} else if (isDigit(*m_currentSymbol)) {

		int res = *m_currentSymbol++ - '0';
			while (isDigit(*m_currentSymbol)) {
				res = res * 10 + ((*m_currentSymbol) - '0');
				m_currentSymbol++;
			}
			//m_currentSymbol--;
			gLogger.log(std::string("Lexer is recognizing number: ") + std::to_string(res));
			token = TokenFabric::createToken<double>(LITERAL, res);

	} else if (isLetter(*m_currentSymbol)) {

		char word[64];
		for(size_t i = 0; isLetter(*m_currentSymbol) || isDigit(*m_currentSymbol); ++i) {
			word[i] = *m_currentSymbol++;
			word[i+1] = '\0';
		}
		std::string sWord(word);
		if (isKeyword(word)) {
			gLogger.log(std::string("Lexer is recognizing keyword: ") + sWord);
			token = TokenFabric::createToken<KeywordMnemonic>(KEYWORD, recognizeKeywordMnemonic(word));
		} else {
			gLogger.log(std::string("Lexer is recognizing id: ") + sWord);
			token = TokenFabric::createToken<std::string>(ID, sWord);
		}

	} else {
		gLogger.log(std::string("Couldn't recognize char: ") + *m_currentSymbol++, WARNING);
		gLogger.log("Skipping unknown token...");
	}

	return token;

}



