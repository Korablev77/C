#ifndef PARSER_HPP
#define PARSER_HPP

#include "Exception.hpp"
#include "TokenHolder.hpp"
#include "Logger.hpp"
#include "ASTtree.hpp"

//GRAMMAR:
//N = [0-9]+
//E = T{[+-]T}*
//T = P{[*/]P}*
//P = {-}(E) |M | F 
//M = D | V
//V = [a-z]+
//D = N.N | N
//S = V=E


static bool checkSemantic(const ASTnode* root);

class Parser {
public:
	Parser();

	void setTokenHolder(TokenHolder* newHolder);

	ASTree* buildAST();

	~Parser();

private:
	Token* getCurrentToken() const;
	void switchToNextToken() const;

	ASTnode* getS(); //Statement
	ASTnode* getV(); //Variable
	ASTnode* getN(); //Natural number
	ASTnode* getM(); //Unary minus
	ASTnode* getF(); //Function
	ASTnode* getE(); //Expression
	ASTnode* getT(); //Term
	ASTnode* getP(); //Production
	ASTnode* getA();

	TokenHolder *m_holder;
};


#endif


