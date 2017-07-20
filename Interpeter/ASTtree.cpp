#include "include/ASTtree.hpp"



BinaryOperator::BinaryOperator(OPERATOR::BinaryOperatorType op, ASTnode* lhs, ASTnode* rhs) : 
	m_operator(op), m_lhs(lhs), m_rhs(rhs) {
	ASSERT((nullptr != m_lhs) && "Binary operator couldn't have null-lhs.");
	ASSERT((nullptr != m_rhs) && "Binary operator couldn't have null-rhs.");
}


ASTnodeType BinaryOperator::getNodeType() const {
	return BINARY_OPERATOR;
}

OPERATOR::BinaryOperatorType BinaryOperator::getOperatorType() const {
	return m_operator;
}

ASTnode* BinaryOperator::getLHS() {
	return m_lhs;
}

ASTnode* BinaryOperator::getRHS() {
	return m_rhs;
}

void BinaryOperator::setLHS(ASTnode* lhs) {
	m_lhs = lhs;
} 

void BinaryOperator::setRHS(ASTnode* rhs) {
	m_rhs = rhs;
}

std::string BinaryOperator::toString() const {

	switch (m_operator) {

		case OPERATOR::ADDITION       : return std::string("+");
		case OPERATOR::SUBSTRACTION   : return std::string("-");
		case OPERATOR::MULTIPLICATION : return std::string("*");
		case OPERATOR::DIVISION       : return std::string("/");
		case OPERATOR::EQUALITY       : return std::string(" = ");
		default 				      : return std::string("Unknown binary operator");
	}
}

BinaryOperator* BinaryOperator::clone() const {
	return new BinaryOperator(*this);
}

bool BinaryOperator::operator==(const ASTnode& other) const {
	if (BINARY_OPERATOR != other.getNodeType()) return false;
	const BinaryOperator* op = static_cast<const BinaryOperator*>(&other);
	if (m_operator != op->getOperatorType()) return false;
	return true;
}


BinaryOperator::~BinaryOperator() {
	gLogger.log(std::string("Deleting binary operator with address: ") + std::to_string(long(this)));
	gLogger.log(std::string("And lhs: ") + std::to_string(long(m_lhs)));
	gLogger.log(std::string("And rhs: ") + std::to_string(long(m_rhs)));

	//delete m_lhs;
	//delete m_rhs;
}



UnaryOperator::UnaryOperator(OPERATOR::UnaryOperatorType op, ASTnode* argument) :
	m_operator(op), m_argument(argument) {
	ASSERT((nullptr != m_argument) && "Unary operator couldn't have null-argument");
}

ASTnodeType UnaryOperator::getNodeType() const {
	return UNARY_OPERATOR;
}

OPERATOR::UnaryOperatorType UnaryOperator::getOperatorType() const {
	return m_operator;
}

ASTnode* UnaryOperator::getArgument() {
	return m_argument;
}

void UnaryOperator::setArgument(ASTnode *newArg) {
	m_argument = newArg; 
}

std::string UnaryOperator::toString() const {

	switch (m_operator) {
		case OPERATOR::MINUS : return std::string("-");
		case OPERATOR::PLUS  : return std::string("+");
		default              : return std::string("Unknown unary operator");
	}
}

UnaryOperator* UnaryOperator::clone() const {
	return new UnaryOperator(*this);
}

bool UnaryOperator::operator==(const ASTnode& other) const {
	return true;
}

UnaryOperator::~UnaryOperator() {
	gLogger.log(std::string("Deleting unary operator with address: ") + std::to_string(long(this)));
	gLogger.log(std::string("And argument: ") + std::to_string(long(m_argument)));
	//delete m_argument;
}



Number::Number(const double value) : m_value(value) {}

ASTnodeType Number::getNodeType() const {
	return NUMBER;
}

double Number::getValue() const {
	return m_value;
}

void Number::setValue(double newVal) {
	m_value = newVal;
}

std::string Number::toString() const {
	return std::to_string((int)m_value);
}

Number* Number::clone() const {
	return new Number(*this);
}

bool Number::operator==(const ASTnode& other) const {
	return true;
}


Variable::Variable(const std::string &name) : m_name(name) {}

ASTnodeType Variable::getNodeType() const {
	return VARIABLE;
}

std::string Variable::getName() const {
	return m_name;
}

std::string Variable::toString() const {
	return m_name;
}

Variable* Variable::clone() const {
	return new Variable(*this);
}

bool Variable::operator==(const ASTnode& other) const {
	return true;
}

Function::Function(const std::string& name, ASTnode* argument) :
	m_name(name), m_argument(argument) {
	ASSERT((nullptr != m_argument) && "Function couldn't have null-argument");
}

ASTnodeType Function::getNodeType() const {
	return FUNCTION;
}

std::string Function::getName() const {
	return m_name;
}

ASTnode* Function::getArgument() {
	return m_argument;
}

void Function::setArgument(ASTnode* newArg) {
	m_argument = newArg;
}

std::string Function::toString() const {
	return m_name;
}

Function* Function::clone() const {
	return new Function(*this);
}

bool Function::operator==(const ASTnode& other) const {
	return true;
}


ASTree::ASTree(ASTnode* root) : m_root(root) {
	ASSERT((nullptr != root) && "Couldnt' create tree with null-root.");
}

ASTree* ASTree::createTree(ASTnode* root) {
	return new ASTree(root);
}

const ASTnode* ASTree::getRootNode() const {
	return m_root;
}

void ASTree::dump(ASTnode* node, bool isRoot /* = false*/) const {
	if (isRoot) { 
		gLogger.log("=== AST DUMP ===");
		node = m_root;
	}
	ASSERT((nullptr != m_root) && "Root was corrupted.");
	ASSERT((nullptr != node)   && "Couldn't dump null-node.");

	switch (node->getNodeType()) {
		case BINARY_OPERATOR : {
			BinaryOperator* op = static_cast<BinaryOperator*>(node);
			gLogger.log("LHS: ");
			dump(op->getLHS());
			gLogger.log("Binary op: ");
			gLogger.log(op->toString());
			gLogger.log("RHS: ");
			dump(op->getRHS());
			break;
		}
		case UNARY_OPERATOR : {
			UnaryOperator* op = static_cast<UnaryOperator*>(node);
			gLogger.log("Unary op: ");
			gLogger.log(op->toString());
			gLogger.log("Argument: ");
			dump(op->getArgument());
			break;
		}
		case NUMBER : {
			Number* numb = static_cast<Number*>(node);
			gLogger.log(std::to_string(numb->getValue()));
			break;
		}
		case VARIABLE : { 
			Variable* var = static_cast<Variable*>(node);
			gLogger.log(var->getName());
			break;
		}
		case FUNCTION : {
			Function* func = static_cast<Function*>(node);
			gLogger.log(func->getName());
			break;
		}
		default:
			gLogger.log("Couldn't print unknown node.", WARNING);
			break;
	}

	if (isRoot) {
		gLogger.log(std::string("Root is ") + m_root->toString());
		gLogger.log("=== DUMP END ===");
	}
}

std::string toStringRecursive(const ASTnode* const node) {
	std::string resStr;

	switch (node->getNodeType()) {
		case BINARY_OPERATOR : {
			BinaryOperator* op = const_cast<BinaryOperator*>(static_cast<const BinaryOperator*>(node));
			resStr += "(";
			resStr += toStringRecursive(op->getLHS());
			resStr += op->toString();
			resStr += toStringRecursive(op->getRHS());
			resStr += ")";
			break;
		}
		case UNARY_OPERATOR : {
			UnaryOperator* op = const_cast<UnaryOperator*>(static_cast<const UnaryOperator*>(node));
			resStr += "(";
			resStr += op->toString();
			resStr += toStringRecursive(op->getArgument());
			resStr += ")";
			break;
		}
		case NUMBER : {
			const Number* numb = static_cast<const Number*>(node);
			resStr += numb->toString();
			break;
		}
		case VARIABLE : { 
			const Variable* var = static_cast<const Variable*>(node);
			resStr += var->toString();
			break;
		}
		case FUNCTION : {
			Function* func = const_cast<Function*>(static_cast<const Function*>(node));
			resStr += func->toString();
			resStr += "(";
			resStr += toStringRecursive(func->getArgument());
			resStr += ")";
			break;
		}
		default:
			gLogger.log("Couldn't print unknown node.", WARNING);
			break;
	}
	return resStr;

}


std::string ASTree::toString() const {
	return toStringRecursive(m_root);
}

void recursiveDelete(ASTnode* node) {
	ASSERT((nullptr != node) && "Attempt at destroying null-node.");

	switch (node->getNodeType()) {
		case BINARY_OPERATOR :
			recursiveDelete(static_cast<BinaryOperator*>(node)->getLHS());
			recursiveDelete(static_cast<BinaryOperator*>(node)->getRHS());
			break;
		case UNARY_OPERATOR :
			recursiveDelete(static_cast<UnaryOperator*>(node)->getArgument());
			break;
		case FUNCTION:
			recursiveDelete(static_cast<Function*>(node)->getArgument());
			break;
		default :
			break;

	}
	delete node;
	node = nullptr;
}


ASTree::~ASTree() {
	recursiveDelete(m_root);
}

OPERATOR::BinaryOperatorType convertSymbolToBinaryOperator(SymbolMnemonic mnemonic) {
	switch (mnemonic) {
		case PLUS  : return OPERATOR::ADDITION;
		case MINUS : return OPERATOR::SUBSTRACTION;
		case STAR  : return OPERATOR::MULTIPLICATION;
		case SLASH : return OPERATOR::DIVISION;
		default    : return OPERATOR::UNKNOWN;
	}
}


bool equals(const ASTnode* const lhs, const ASTnode* const rhs) {

	if (lhs->getNodeType() != rhs->getNodeType()) return false;

	return false;
}

ASTnode* copyTree(const ASTnode* root) {
	ASSERT((nullptr != root) && "Couldn't copy null-node.");
	ASTnode* newRoot = root->clone();

	switch (newRoot->getNodeType()) {
		case BINARY_OPERATOR : 
			((BinaryOperator*)newRoot)->setLHS(copyTree(((BinaryOperator*)root)->getLHS()));
			((BinaryOperator*)newRoot)->setRHS(copyTree(((BinaryOperator*)root)->getRHS()));
			break;
		case UNARY_OPERATOR : 
			((UnaryOperator*)newRoot)->setArgument(copyTree(((UnaryOperator*)root)->getArgument()));
			break;
		case FUNCTION : 
			((Function*)newRoot)->setArgument(copyTree(((Function*)root)->getArgument()));
			break;
		default: break;
	}

	return newRoot;
}

void ASTree::eval(ASTnode** node) {
	ASSERT((nullptr !=  node) && "Address of pointer to node couldn't be null.");
	ASSERT((nullptr != *node) && "Couldn't eval null-node.");

	switch ((*node)->getNodeType()) {
		case BINARY_OPERATOR : {
			BinaryOperator* binOp = static_cast<BinaryOperator*>(*node);
			ASTnode* lhs = binOp->getLHS();
			ASTnode* rhs = binOp->getRHS();
			eval(&lhs);
			eval(&rhs);
			binOp->setLHS(lhs);
			binOp->setRHS(rhs);
			break;
		}
		case UNARY_OPERATOR : {
			UnaryOperator* unOp = static_cast<UnaryOperator*>(*node);
			ASTnode* arg = unOp->getArgument();
			eval(&arg);
			unOp->setArgument(arg);
			break;
		}
		case FUNCTION : {
			Function* func = static_cast<Function*>(*node);
			ASTnode* arg = func->getArgument();
			eval(&arg);
			if ("d" == func->getName()) {
				derivative(&arg);
				delete (*node);
				*node = arg;
			} else {
				func->setArgument(arg);
			}
			break;
		}
		default: break;
	}
}

void ASTree::substituteVariables(ASTnode** node) {
	ASSERT((nullptr !=  node) && "Address of pointer to node couldn't be null.");
	ASSERT((nullptr != *node) && "Couldn't derive null-node.");

	switch ((*node)->getNodeType()) {
		case BINARY_OPERATOR : {
			BinaryOperator* binOp = static_cast<BinaryOperator*>(*node);
			ASTnode* lhs = binOp->getLHS();
			ASTnode* rhs = binOp->getRHS();
			substituteVariables(&rhs);
			if (OPERATOR::EQUALITY != binOp->getOperatorType()) {
				substituteVariables(&lhs);
				binOp->setLHS(lhs);
			}
			substituteVariables(&rhs);
			binOp->setRHS(rhs);
			break;
		}
		case UNARY_OPERATOR : {
			UnaryOperator* unOp = static_cast<UnaryOperator*>(*node);
			ASTnode* arg = unOp->getArgument();
			substituteVariables(&arg);
			unOp->setArgument(arg);
			break;
		}
		case FUNCTION : {
			Function* func = static_cast<Function*>(*node);
			ASTnode* arg = func->getArgument();
			substituteVariables(&arg);
			func->setArgument(arg);
			break;
		}
		case VARIABLE : {
			Variable* var = static_cast<Variable*>(*node);
			IdValue idValue = TokenHolder::getSymbolValue(var->getName());
			if (IdValue::DOUBLE == idValue.getType()) {
				double value = idValue.value();
				if (value != TokenHolder::SYMBOL_NOT_FOUND) {
					gLogger.log(std::string("Substituting variable ")+ var->getName() + 
						std::string(" with value ") + std::to_string(int(value)));
					delete *node;
					*node = new Number(value);
				} 
			} else {
					ASTnode* branch = idValue.root();
					delete *node;
					*node = copyTree(branch);
				}
			}	
		
		default: break;
	}
}


bool isNativeFunction(std::string funcName) {
	for (auto name : FUNCTIONS) {
		if (funcName == name) return true;
	}
	return false;
}

static void diffLog(ASTnode** node) {
	ASSERT((nullptr !=  node) && "Address of pointer to node couldn't be null.");
	ASSERT((nullptr != *node) && "Couldn't derive null-node.");

	ASTnode* lhs = new Number(1.0);
	ASTnode* rhs = copyTree(*node);

	recursiveDelete(*node);
	*node = new BinaryOperator(OPERATOR::DIVISION, lhs, rhs);
}

static void diffSin(ASTnode** node) {
	ASSERT((nullptr !=  node) && "Address of pointer to node couldn't be null.");
	ASSERT((nullptr != *node) && "Couldn't derive null-node.");

	ASTnode* arg = copyTree(*node);

	recursiveDelete(*node);
	*node = new Function("cos", arg);
}

static void diffCos(ASTnode** node) {
	ASSERT((nullptr !=  node) && "Address of pointer to node couldn't be null.");
	ASSERT((nullptr != *node) && "Couldn't derive null-node.");

	ASTnode* arg = copyTree(*node);

	recursiveDelete(*node);
	ASTnode* func = new Function("sin", arg);
	*node = new UnaryOperator(OPERATOR::MINUS, func);

}

static void diffExp(ASTnode** node) {
	ASSERT((nullptr !=  node) && "Address of pointer to node couldn't be null.");
	ASSERT((nullptr != *node) && "Couldn't derive null-node.");
	
	ASTnode* arg = copyTree(*node);

	recursiveDelete(*node);
	*node = new Function("exp", arg);
}



void ASTree::derivative(ASTnode** node) {
	ASSERT((nullptr !=  node) && "Address of pointer to node couldn't be null.");
	ASSERT((nullptr != *node) && "Couldn't derive null-node.");
	
	switch((*node)->getNodeType()) {
		case BINARY_OPERATOR : {
			BinaryOperator* binOp = static_cast<BinaryOperator*>(*node);
			gLogger.log(std::string("Differentiate binary operator ") + binOp->toString());
			ASTnode* lhs = binOp->getLHS();
			ASTnode* rhs = binOp->getRHS();
			if (OPERATOR::MULTIPLICATION == binOp->getOperatorType()) {
				ASTnode* llhs = copyTree(lhs);
				ASTnode* lrhs = copyTree(rhs);
				derivative(&llhs);
				ASTnode* lMul = new BinaryOperator(OPERATOR::MULTIPLICATION, llhs, lrhs);

				ASTnode* rlhs = copyTree(lhs);
				ASTnode* rrhs =	copyTree(rhs);
				derivative(&rrhs);
				ASTnode* rMul = new BinaryOperator(OPERATOR::MULTIPLICATION, rlhs, rrhs);

				recursiveDelete(*node);
				*node = new BinaryOperator(OPERATOR::ADDITION, lMul, rMul);
			} else if (OPERATOR::DIVISION == binOp->getOperatorType()) {
				ASTnode* nominatorLlhs = copyTree(lhs);
				ASTnode* nominatorLrhs = copyTree(rhs);
				derivative(&nominatorLlhs);
				ASTnode* nominatorLmul = new BinaryOperator(
					OPERATOR::MULTIPLICATION, nominatorLlhs, nominatorLrhs);

				ASTnode* nominatorRlhs = copyTree(lhs);
				ASTnode* nominatorRrhs = copyTree(rhs);
				derivative(&nominatorRrhs);
				ASTnode* nominatorRmul = new BinaryOperator(
					OPERATOR::MULTIPLICATION, nominatorRlhs, nominatorRrhs);

				ASTnode* nominator = new BinaryOperator(
					OPERATOR::SUBSTRACTION, nominatorLmul, nominatorRmul);

				ASTnode* denominatorLhs = copyTree(rhs);
				ASTnode* denominatorRhs = copyTree(rhs);
				ASTnode* denominator = new BinaryOperator(
					OPERATOR::MULTIPLICATION, denominatorLhs, denominatorRhs);

				recursiveDelete(*node);
				*node = new BinaryOperator(OPERATOR::DIVISION,
					nominator, denominator);

			} else {
				derivative(&lhs);
				derivative(&rhs);
				binOp->setLHS(lhs);
				binOp->setRHS(rhs);
			}
			break;
		}
		case UNARY_OPERATOR : {
			UnaryOperator* unOp = static_cast<UnaryOperator*>(*node);
			gLogger.log(std::string("Differentiate unary operator ") + unOp->toString());
			ASTnode* arg = unOp->getArgument();
			derivative(&arg);
			unOp->setArgument(arg);
			break;
		}
		case NUMBER : {
			Number* numb = static_cast<Number*>(*node);
			numb->setValue(0.0);
			break;
		}
		case VARIABLE : {
			delete *node;
			*node = new Number(1);
			break;
		}
		case FUNCTION : {
			Function* func = static_cast<Function*>(*node);
			gLogger.log("Differentiate complex function " +toStringRecursive(func));
			ASTnode*  lhs  = copyTree(func->getArgument());
			ASTnode*  rhs  = copyTree(func->getArgument());
 			derivative(&lhs);
 			gLogger.log(std::string("Argument after differentiation") + toStringRecursive(lhs));
 			std::string funcName = func->getName();
 			if (isNativeFunction(funcName)) {
 				if ("log" == funcName) {
 					gLogger.log("Differentiate log...");
 					diffLog(&rhs);
 				} else if ("sin" == funcName) {
 					gLogger.log("Differentiate sin...");
 					diffSin(&rhs);
 				} else if ("cos" == funcName) {
 					diffCos(&rhs);
 				} else if ("exp" == funcName) {
 					diffExp(&rhs);	
 				}
 			} else {
 				THROW_EXCEPTION(BaseException, std::string("Couldn't differentiate unknown function: ") + funcName);
 			}

 			recursiveDelete(*node);
 			*node = new BinaryOperator(OPERATOR::MULTIPLICATION, lhs, rhs);
 			break;
		}
		default : 
			gLogger.log("Couldn't differentiate unknown node.", WARNING);
	}
}

//while tree doesn't change -- simplify in loop
void ASTree::simplify() {	
	//Node<Token>* beforeSimplifications = NULL;
	//do{
	//	delete beforeSimplifications;
	//	beforeSimplifications = copyTree(m_root);
		substituteVariables(&m_root);
		eval(&m_root);
		simplifyLiterals(&m_root);
		simplifyOneMul(&m_root);
		simplifyZeroMul(&m_root);
		simplifyLiterals(&m_root);
		simplifyZeroAdd(&m_root);

	//} while(equals(beforeSimplifications));
	//delete beforeSimplifications;
}

void ASTree::simplifyLiterals(ASTnode** node) {

	ASSERT((nullptr !=  node) && "Address of pointer to node couldn't be null.");
	ASSERT((nullptr != *node) && "Couldn't simplify null-node.");


	if (BINARY_OPERATOR == (*node)->getNodeType()) {
		BinaryOperator* binOp = static_cast<BinaryOperator*>(*node);
		gLogger.log(std::string("Binary operator") + binOp->toString());
		ASTnode* lhs = binOp->getLHS();
		ASTnode* rhs = binOp->getRHS();
		gLogger.log(std::string("LHS:") + std::to_string((long)lhs));
		gLogger.log(std::string("RHS:") + std::to_string((long)rhs));
		gLogger.log(std::string("LHS addr:") + std::to_string((long)&lhs));
		gLogger.log(std::string("RHS addr:") + std::to_string((long)&rhs));
		simplifyLiterals(&lhs);
		simplifyLiterals(&rhs);
		binOp->setLHS(lhs);
		binOp->setRHS(rhs);
		if (NUMBER == lhs->getNodeType() && 
			NUMBER == rhs->getNodeType()) { 
			gLogger.log(std::string("Simplifying literals with binary operator: ") + binOp->toString());
			double lhsVal = static_cast<Number*>(lhs)->getValue();
			double rhsVal = static_cast<Number*>(rhs)->getValue();
			gLogger.log(std::string("LHS value ") + std::to_string(int(lhsVal)));
			gLogger.log(std::string("RHS value ") + std::to_string(int(rhsVal)));
			double res = 0;
			switch (binOp->getOperatorType()) {
				case OPERATOR::ADDITION : 
					res = lhsVal + rhsVal;
					break;
				case OPERATOR::SUBSTRACTION :
					res = lhsVal - rhsVal;
					break;
				case OPERATOR::MULTIPLICATION :
					res = lhsVal * rhsVal;
					break;
				case OPERATOR::DIVISION :
					res = lhsVal / rhsVal;
					break;
				default:
					gLogger.log(std::string("Couldn't simplify operator node: " ) + binOp->toString(), WARNING);
					return;
			}
			delete lhs;
			delete rhs;
			delete (*node);
			gLogger.log(std::string("Result after simplification: ") + std::to_string(int(res)));
			*node = new Number(res);
		} 
	} else if (UNARY_OPERATOR == (*node)->getNodeType()) {
		UnaryOperator* unOp = static_cast<UnaryOperator*>(*node);
		ASTnode* opArg = unOp->getArgument(); 
		gLogger.log(std::string("Arg:") + std::to_string((long)opArg));
		gLogger.log(std::string("Arg addr:") + std::to_string((long)&opArg));
		simplifyLiterals(&opArg);
		if (NUMBER == opArg->getNodeType()) {
			gLogger.log(std::string("Simplifying literal with unary operator: ") + unOp->toString());
			double arg = static_cast<Number*>(opArg)->getValue();
			double newArg = 0;
			switch (unOp->getOperatorType()) {
				case OPERATOR::PLUS :
					newArg = arg;
					break;
				case OPERATOR::MINUS :
					newArg = - arg;
					break;
				default:
					gLogger.log(std::string("Couldn't simplify operator node: ") + unOp->toString(), WARNING);
					return;
			}
			delete (*node);
			gLogger.log(std::string("Result after simplification: ") + std::to_string(int(newArg)));
			*node = new Number(newArg);
		} else {
			unOp->setArgument(opArg);
		}
	} else if (FUNCTION == (*node)->getNodeType()) {
		Function* func = static_cast<Function*>(*node);
		ASTnode* opArg = func->getArgument(); 
		simplifyLiterals(&opArg);
		func->setArgument(opArg);

	}
}

void ASTree::simplifyZeroMul(ASTnode** node) {
	ASSERT((nullptr != node)  && "Address of pointer to node couldn't be null.");
	ASSERT((nullptr != *node) && "Couldn't simplify null-node.");

	if (BINARY_OPERATOR == (*node)->getNodeType()) {
		BinaryOperator* binOp = static_cast<BinaryOperator*>(*node);
		ASTnode* lhs = binOp->getLHS();
		ASTnode* rhs = binOp->getRHS();
		simplifyZeroMul(&lhs);
		simplifyZeroMul(&rhs);
		binOp->setLHS(lhs);
		binOp->setRHS(rhs);

		if (OPERATOR::MULTIPLICATION == binOp->getOperatorType() &&
			 (((NUMBER == lhs->getNodeType()) && (0.0 == ((Number*)lhs)->getValue())) || 
				((NUMBER == rhs->getNodeType()) && (0.0 == ((Number*)rhs)->getValue())))) {
			gLogger.log("Simplifying multplication by zero...");
			recursiveDelete(lhs);
			recursiveDelete(rhs);
			delete (*node);
			*node = new Number(0.0);
		}
	}
}

void ASTree::simplifyZeroAdd(ASTnode** node) {

	ASSERT((nullptr != node)  && "Address of pointer to node couldn't be null.");
	ASSERT((nullptr != *node) && "Couldn't simplify null-node.");

	if (BINARY_OPERATOR == (*node)->getNodeType()) {
		BinaryOperator* binOp = static_cast<BinaryOperator*>(*node);
		ASTnode* lhs = binOp->getLHS();
		ASTnode* rhs = binOp->getRHS();
		simplifyZeroAdd(&lhs);
		simplifyZeroAdd(&rhs);
		binOp->setLHS(lhs);
		binOp->setRHS(rhs);

		if (OPERATOR::ADDITION == binOp->getOperatorType() &&
			 (((NUMBER == lhs->getNodeType()) && (0.0 == ((Number*)lhs)->getValue())))) {
			gLogger.log("Simplifying addition with zero...");
			delete lhs;
			delete (*node);
			*node = rhs;
		} else if (OPERATOR::ADDITION == binOp->getOperatorType() &&
			 (((NUMBER == rhs->getNodeType()) && (0.0 == ((Number*)rhs)->getValue())))) {
			gLogger.log("Simplifying addition with zero...");
			delete rhs;
			delete (*node);
			*node = lhs;
		} else if (OPERATOR::SUBSTRACTION == binOp->getOperatorType() &&
			 (((NUMBER == rhs->getNodeType()) && (0.0 == ((Number*)rhs)->getValue())))) {
			gLogger.log("Simplifying substraction with zero...");
			delete rhs;
			delete (*node);
			*node = lhs;
		} else if (OPERATOR::SUBSTRACTION == binOp->getOperatorType() &&
			(((NUMBER == lhs->getNodeType()) && (0.0 == ((Number*)lhs)->getValue())))) {
			gLogger.log("Simplifying substraction with zero...");
			delete lhs;
			delete (*node);
			*node = new UnaryOperator(OPERATOR::MINUS, rhs);
		}
	}

}


void ASTree::simplifyOneMul(ASTnode** node) {
	ASSERT((nullptr != node)  && "Address of pointer to node couldn't be null.");
	ASSERT((nullptr != *node) && "Couldn't simplify null-node.");

	if (BINARY_OPERATOR == (*node)->getNodeType()) {
		BinaryOperator* binOp = static_cast<BinaryOperator*>(*node);
		ASTnode* lhs = binOp->getLHS();
		ASTnode* rhs = binOp->getRHS();
		simplifyOneMul(&lhs);
		simplifyOneMul(&rhs);
		binOp->setLHS(lhs);
		binOp->setRHS(rhs);

		if (OPERATOR::MULTIPLICATION == binOp->getOperatorType() &&
			 (((NUMBER == lhs->getNodeType()) && (1 == ((Number*)lhs)->getValue())))) {
			gLogger.log("Simplifying multplication by one...");
			delete lhs;
			delete (*node);
			*node = rhs;
		}
		else if (OPERATOR::MULTIPLICATION == binOp->getOperatorType() &&
			 	  (((NUMBER == rhs->getNodeType()) && (1 == ((Number*)rhs)->getValue())))) {
			gLogger.log("Simplifying multplication by one...");
			delete rhs;
			delete (*node);
			*node = lhs;
		}
	}
}



