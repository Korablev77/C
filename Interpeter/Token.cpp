#include "include/Token.hpp"


Token::Token() {}



Symbol::Symbol(SymbolMnemonic mnemonic) : m_mnemonic(mnemonic) {}

TokenType Symbol::getTokenType() const {
	return SYMBOL;
}

SymbolMnemonic Symbol::getMnemonic() const {
	return m_mnemonic;
}

std::string Symbol::toString() const {
	switch (m_mnemonic) {
		case PLUS          : return std::string("+");
		case MINUS         : return std::string("-");
		case EQUALITY      : return std::string("=");
		case SLASH         : return std::string("/");
		case STAR          : return std::string("*");
		case DOT           : return std::string(".");
		case COMMA         : return std::string(",");
		case OPEN_BRACKET  : return std::string("(");
		case CLOSE_BRACKET : return std::string(")");
		case COLON         : return std::string(":");
		case SEMICOLON     : return std::string(";");
		case EOL           : return std::string("\0");
		default : return std::string("Unknown Symbol");
	}	
}




Literal::Literal(double value) : m_value(value) {}

TokenType Literal::getTokenType() const {
	return LITERAL;
}

double Literal::getValue() const {
	return m_value;
}

std::string Literal::toString() const {
	return std::string(std::to_string(m_value));
}




Id::Id(std::string &name) : m_name(name) {}

TokenType Id::getTokenType() const {
	return ID;
}

std::string Id::toString() const {
	return m_name;
}



Keyword::Keyword(KeywordMnemonic mnemonic) : m_mnemonic(mnemonic) {}

TokenType Keyword::getTokenType() const {
	return KEYWORD;
}

std::string Keyword::toString() const {
	return std::string("Keyword");
}

KeywordMnemonic Keyword::getMnemonic() const {
	return m_mnemonic;
}


SymbolMnemonic recognizeSymbolMnemonic(const char symbol) {
	switch (symbol) {
		case '+' : return PLUS;
		case '-' : return MINUS;
		case '=' : return EQUALITY;
		case '/' : return SLASH;
		case '*' : return STAR;
		case '.' : return DOT;
		case ',' : return COMMA;
		case '(' : return OPEN_BRACKET;
		case ')' : return CLOSE_BRACKET;
		case ':' : return COLON;
		case ';' : return SEMICOLON;
		case '\0': return EOL;
		default  : return UNKNOWN_SYMBOL;
	} 
}

KeywordMnemonic recognizeKeywordMnemonic(const char* keyword) {
	if (strcmp(keyword, "if"))    return IF;
	if (strcmp(keyword, "ELSE"))  return ELSE;
	if (strcmp(keyword, "DO"))    return DO;
	if (strcmp(keyword, "WHILE")) return WHILE;
	return UNKNOWN_KEYWORD;
}



