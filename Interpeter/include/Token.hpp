#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>


#include "Exception.hpp"
#include "Constants.hpp"
//#include "debug.h"


typedef enum TokenType {
	SYMBOL = 0,
	LITERAL = 1,
	ID = 2,
	KEYWORD = 3
}TokenType;

typedef enum SymbolMnemonic {
	DOT = 0,
	COMMA = 1, 
	OPEN_BRACKET = 2,
	CLOSE_BRACKET = 3,
	SEMICOLON = 4,
	COLON = 5,
	PLUS = 6,
	MINUS = 7,
	EQUALITY = 8,
	SLASH = 9,
	STAR = 10,
	EOL = 11,
	UNKNOWN_SYMBOL

}SymbolMnemonic;

typedef enum KeywordMnemonic {
	IF = 0,
	ELSE = 1,
	DO = 2,
	WHILE = 3,
	UNKNOWN_KEYWORD
}KeywordMnemonic;




class Token {
public:
	Token();
	virtual TokenType getTokenType() const = 0;
	virtual std::string toString() const = 0;

	virtual ~Token() {};
};



class Symbol : public Token {
public:
	explicit Symbol(SymbolMnemonic mnemonic);

	virtual TokenType getTokenType() const override;

	SymbolMnemonic getMnemonic() const;

	virtual std::string toString() const override;

	~Symbol() = default;

private:
	SymbolMnemonic m_mnemonic;
};

class Literal : public Token {
public:
	explicit Literal(double value);

	virtual TokenType getTokenType() const override;

	double getValue() const;

	virtual std::string toString() const override;

	~Literal() = default;

private:
	double m_value;
};


class Id : public Token {
public:
	explicit Id(std::string &name);
	
	virtual TokenType getTokenType() const override;

	virtual std::string toString() const override;

	~Id() = default;

private:
	std::string m_name;
};


class Keyword : public Token {
public:
	explicit Keyword(KeywordMnemonic mnemonic);

	virtual TokenType getTokenType() const override;

	//TODO: implementation
	virtual std::string toString() const override;

	KeywordMnemonic getMnemonic() const;

	~Keyword() = default;

private:
	KeywordMnemonic m_mnemonic;
};



SymbolMnemonic  recognizeSymbolMnemonic(const char symbol);

KeywordMnemonic recognizeKeywordMnemonic(const char* keyword); 


template<typename T>
struct Identity {};

class TokenFabric {
public:

	template<typename T>
	static Token* createToken(TokenType type, T value) {
		return createToken(type, value, Identity<T>());
	}
private:
	template<typename T>
	static Token* createToken(TokenType type, T value, Identity<T> ) {
		THROW_EXCEPTION(BaseException, "Couldn't create unknown token");
	}

	static Token* createToken(TokenType type, SymbolMnemonic mnemonic, Identity<SymbolMnemonic>) {
		return new Symbol(mnemonic);
	}

	static Token* createToken(TokenType type, double value, Identity<double>) {
		return new Literal(value);
	}

	static Token* createToken(TokenType type, std::string& name, Identity<std::string>) {
		return new Id(name);
	}

	static Token* createToken(TokenType type, KeywordMnemonic mnemonic, Identity<KeywordMnemonic>) {
		return new Keyword(mnemonic);
	}
};

#endif
