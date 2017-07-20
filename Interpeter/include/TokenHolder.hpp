#ifndef TOKENHOLDER_HPP
#define TOKENHOLDER_HPP

#include <assert.h>
#include <stdio.h>
#include <map>
#include <string>
#include <vector>
#include "ASTtree.hpp"
#include "Token.hpp"
#include "Helpers.hpp"
#include "Logger.hpp"
#include "Exception.hpp"

//forward declration
class ASTnode;

struct IdValue {
public:
	enum IdValueType {DOUBLE, TREE};
	explicit IdValue(const double value);
	explicit IdValue(ASTnode* const root);

	double&  value();
	ASTnode*& root();

	IdValueType getType() const;
private:
	void check(IdValueType type) const;

	union {
		double   m_value;
		ASTnode* m_root;
	};
	IdValueType m_type;
};

class TokenHolder {
public:
	TokenHolder(const std::vector<Token*> &tokens);

	Token* getCurrentToken() const;

	bool switchToNextToken();

	static void addSymbol(const std::string &id, const IdValue value);

	static IdValue getSymbolValue(const std::string &id);

	static void undef(const std::string &id = "all");

	static void printSymbolTable();

	void printTokens();

	static double SYMBOL_NOT_FOUND;

	~TokenHolder();

private:
	static std::map<std::string, IdValue> createSymbolTable();

	std::vector<Token*> m_tokens;
	size_t m_currentPosition;

	static std::map<std::string, IdValue> m_symbolTable;
};


#endif

