
#include "include/Parser.hpp"

//2x^2+3x -> (+ (* 2 (^x 2))(* 3 x)) dump

Parser::Parser() : m_holder(nullptr) {}

void Parser::setTokenHolder(TokenHolder* newHolder) {
	ASSERT((nullptr != newHolder) && "TokenHolder couldn't be null.");
	delete m_holder;
	m_holder = newHolder;
}


ASTree* Parser::buildAST() {
	ASSERT(m_holder);
	m_holder->printTokens();
	gLogger.log("=== Building AST ===");
	ASTnode *treeRoot = nullptr;
	try {
		treeRoot = getA();
		if (SYMBOL != getCurrentToken()->getTokenType() || 
			EOL != ((Symbol*)getCurrentToken())->getMnemonic()) {
			THROW_EXCEPTION(InternalTranslationException, 
				"At the end of parsing the EOL token was expected, but was given: ", getCurrentToken());
		}

	} catch(BaseException &e) {
		gLogger.log("Failed to build AST.", ERROR);
		gLogger.log(e.what());
		throw;
	}
	gLogger.log(std::string("=== AST was created. Root is ") + treeRoot->toString());

	gLogger.log("Semantic checking...");
	checkSemantic(treeRoot);

	ASTree* tree = ASTree::createTree(treeRoot);
	tree->simplify();
	const ASTnode* newRoot = tree->getRootNode();
	ASSERT((nullptr != tree) && "New AST couldn't be null.");
	ASSERT((nullptr != newRoot) && "AST root couldn't be null.");

	if (BINARY_OPERATOR == newRoot->getNodeType() &&
		OPERATOR::EQUALITY == ((BinaryOperator*)newRoot)->getOperatorType()) {
		ASTnode* rhs = ((BinaryOperator*)newRoot)->getRHS();
		ASTnode* lhs = ((BinaryOperator*)newRoot)->getLHS();
		if (NUMBER == rhs->getNodeType()) {
			TokenHolder::addSymbol(lhs->toString(), IdValue(((Number*)rhs)->getValue()));
		} else {
			TokenHolder::addSymbol(lhs->toString(), IdValue(copyTree(rhs)));
		}
	}


	return tree;
}

Token* Parser::getCurrentToken() const {
	return m_holder->getCurrentToken();
}

void Parser::switchToNextToken() const {
	if (!m_holder->switchToNextToken()) {
		gLogger.log("Token holder: out of index.", ERROR);
		THROW_EXCEPTION(BaseException, "Attempt at getting token with nonexistent index.");
	}
}

static bool checkSemantic(const ASTnode* root) {
	if (BINARY_OPERATOR == root->getNodeType() &&
		OPERATOR::EQUALITY == ((BinaryOperator*)root)->getOperatorType()) {
		ASTnode* lhs = ((BinaryOperator*)root)->getLHS();
		if (VARIABLE != lhs->getNodeType()) {
			gLogger.log("Semantic error: lhs in the assignment should be variable.", WARNING);
			THROW_EXCEPTION(BaseException, "Only variables could be assignable.");
			return false;
		}
	}

	return true;
}

Parser::~Parser() {
	delete m_holder;
}


ASTnode* Parser::getA() {
	ASTnode* res = getE();
	Token* curTok = getCurrentToken();
	if (SYMBOL == curTok->getTokenType() &&
		EQUALITY == ((Symbol*)curTok)->getMnemonic()) {
		switchToNextToken();
		ASTnode* rhs = getE();
		gLogger.log("Creating equality node...");
		ASTnode* intermediateRes = new BinaryOperator(OPERATOR::EQUALITY, res, rhs);
		res = intermediateRes;
	}
	return res;
}


ASTnode* Parser::getM() {
	Token* curTok = getCurrentToken();

	switch (curTok->getTokenType()) {
		case ID      : return getV();
		case LITERAL : return getN();
		default      : THROW_EXCEPTION(InternalTranslationException, 
			"Terminal was expected, but was given: ", getCurrentToken());
	}
}



ASTnode* Parser::getN() { 
	Token* curTok = getCurrentToken();
	if (nullptr != curTok &&  LITERAL != curTok->getTokenType())
		THROW_EXCEPTION(InternalTranslationException,
			"Translation error: Literal was excepted, but another token was given.", getCurrentToken());
	switchToNextToken();
	gLogger.log("Creating literal node: " + curTok->toString());
	return new Number((static_cast<Literal*>(curTok))->getValue());
}

ASTnode* Parser::getV() {
	Token* curTok = getCurrentToken();
	if (nullptr != curTok && ID != curTok->getTokenType())
		THROW_EXCEPTION(InternalTranslationException,
			"Translation error: Variable was excepted, but another token was given.", getCurrentToken());
	switchToNextToken();
	if (SYMBOL == getCurrentToken()->getTokenType() && 
		OPEN_BRACKET == ((Symbol*)getCurrentToken())->getMnemonic()) {
		return new Function((static_cast<Id*>(curTok))->toString(), getP());
	}
	gLogger.log("Creating variable node: " + curTok->toString());
	return new Variable((static_cast<Id*>(curTok))->toString());
}



ASTnode* Parser:: getE() { 
	ASTnode* res = getT();
	Symbol* curSymbol = static_cast<Symbol*>(getCurrentToken());
	while ((SYMBOL == getCurrentToken()->getTokenType()) && 
			(PLUS == curSymbol->getMnemonic() || MINUS == curSymbol->getMnemonic())) {
		switchToNextToken();
		ASTnode* rhs = getT(); 
		gLogger.log("Creating binary operator node...");
		ASTnode* intermediateRes = new BinaryOperator(
			 convertSymbolToBinaryOperator(curSymbol->getMnemonic()), res, rhs);
		res = intermediateRes;
		curSymbol = static_cast<Symbol*>(getCurrentToken());
	}

	return res;
}

ASTnode* Parser::getT() {
	ASTnode* res = getP();
	Symbol* curSymbol = static_cast<Symbol*>(getCurrentToken());
	while ((SYMBOL == getCurrentToken()->getTokenType()) && 
			(STAR == curSymbol->getMnemonic() || SLASH == curSymbol->getMnemonic())) {
		switchToNextToken();
		ASTnode* rhs = getP();
		gLogger.log("Creating binary operator node...");
		ASTnode* intermediateRes = new BinaryOperator(
			convertSymbolToBinaryOperator(curSymbol->getMnemonic()), res, rhs);
		res = intermediateRes;
		curSymbol = static_cast<Symbol*>(getCurrentToken());
	}

	return res;
}


ASTnode* Parser::getP() {

	if (SYMBOL == getCurrentToken()->getTokenType() && 
			 MINUS == static_cast<Symbol*>(getCurrentToken())->getMnemonic()) {
		switchToNextToken();
		return new UnaryOperator(OPERATOR::MINUS, getP());
	}
	if (SYMBOL == getCurrentToken()->getTokenType() && 
			OPEN_BRACKET == static_cast<Symbol*>(getCurrentToken())->getMnemonic()) {
		//skip bracket
		gLogger.log("Skipping open bracket...");
		switchToNextToken();
		ASTnode* res = getE();
		if (SYMBOL != getCurrentToken()->getTokenType() ||
				CLOSE_BRACKET != static_cast<Symbol*>(getCurrentToken())->getMnemonic())
			THROW_EXCEPTION(InternalTranslationException, "Translation error: Couldn't match open bracket", getCurrentToken());
		gLogger.log("Skipping close bracket...");
		switchToNextToken();
		return res;
	}
	return getM();

}











