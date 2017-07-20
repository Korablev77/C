#ifndef ASTTREE_HPP
#define ASTTREE_HPP


#include "Helpers.hpp"
#include "Token.hpp"
#include "Logger.hpp"
#include "TokenHolder.hpp"

namespace OPERATOR {
	typedef enum BinaryOperatorType {
		ADDITION = 0,
		SUBSTRACTION = 1,
		DIVISION = 2,
		MULTIPLICATION = 3,
		EQUALITY = 4,
		UNKNOWN
	}BinaryOperatorType;

	typedef enum UnaryOperatorType {
		PLUS = 0,
		MINUS = 1,
		STAR = 2,
		AMPERSAND = 3,
		NEGATION = 4
	}UnaryOperatorType;
}
typedef enum ASTnodeType {
	BINARY_OPERATOR = 0,
	UNARY_OPERATOR = 1,
	VARIABLE = 2,
	NUMBER = 3,
	FUNCTION = 4
}ASTnodeType;


OPERATOR::BinaryOperatorType convertSymbolToBinaryOperator(SymbolMnemonic mnemonic);

class ASTnode {
public:
	virtual ASTnodeType getNodeType() const = 0;
	virtual std::string toString() const = 0;
	virtual ASTnode* clone() const = 0;
	virtual bool operator==(const ASTnode& other) const = 0;
	virtual ~ASTnode() = default;

};


class BinaryOperator : public ASTnode {
public:
	BinaryOperator(OPERATOR::BinaryOperatorType op, ASTnode* lhs, ASTnode* rhs);

	ASTnodeType getNodeType() const override;

	OPERATOR::BinaryOperatorType getOperatorType() const;

	ASTnode* getLHS();
	ASTnode* getRHS();

	void setRHS(ASTnode* rhs);
	void setLHS(ASTnode* lhs);

	std::string toString() const override;

	virtual BinaryOperator* clone() const override;

	virtual bool operator==(const ASTnode& other) const override;

	~BinaryOperator();

private:
	OPERATOR::BinaryOperatorType m_operator;

	ASTnode* m_lhs;
	ASTnode* m_rhs;
};


class UnaryOperator : public ASTnode {
public:

	UnaryOperator(OPERATOR::UnaryOperatorType op, ASTnode* argument);

	ASTnodeType getNodeType() const override;

	OPERATOR::UnaryOperatorType getOperatorType() const;

	ASTnode* getArgument();
	void     setArgument(ASTnode* newArg);

	std::string toString() const override;

	virtual UnaryOperator* clone() const override;

	virtual bool operator==(const ASTnode& other) const override;

	~UnaryOperator();
private:
	OPERATOR::UnaryOperatorType m_operator;
	ASTnode* m_argument;
};


class Number : public ASTnode {
public:
	Number(const double value);

	ASTnodeType getNodeType() const override;

	double getValue() const;
	void   setValue(double newVal);

	std::string toString() const override;

	virtual Number* clone() const override;

	virtual bool operator==(const ASTnode& other) const override;

	~Number() = default;

private:
	double m_value;
};


class Variable : public ASTnode {
public:
	Variable(const std::string& name);

	ASTnodeType getNodeType() const override;

	std::string getName() const;

	std::string toString() const override;

	virtual Variable* clone() const override;

	virtual bool operator==(const ASTnode& other) const override;

	~Variable() = default;

private:
	std::string m_name;
};

class Function : public ASTnode {
public:
	Function(const std::string& name, ASTnode* argument);

	ASTnodeType getNodeType() const override;

	std::string getName() const;

	ASTnode* getArgument();
	void     setArgument(ASTnode* newArg);

	std::string toString() const override;

	virtual Function* clone() const override;

	virtual bool operator==(const ASTnode& other) const override;

	~Function() = default;

private:
	std::string m_name;
	ASTnode* m_argument;
};

ASTnode* copyTree(const ASTnode* const root);
void recursiveDelete(ASTnode* node);
std::string toStringRecursive(const ASTnode* const node);
static void diffLog(ASTnode** node);
static void diffSin(ASTnode** node);
static void diffCos(ASTnode** node);
static void diffExp(ASTnode** node);


class ASTree {
public:
	//Alowing create trees only on stack
	static ASTree* createTree(ASTnode* root);

	void simplify();
	void substituteVariables(ASTnode** node);
	void eval(ASTnode** node);
	void derivative(ASTnode** node);

	const ASTnode* getRootNode() const;


	void dump(ASTnode* node = nullptr, bool isRoot = false) const;
	std::string toString() const;

	friend bool equals(const ASTnode* const lhs, const ASTnode* const rhs);

	~ASTree();

private:

	ASTree(ASTnode* root);

	ASTree() = delete;
    ASTree(const ASTree&) = delete;
    ASTree& operator=(const ASTree&) = delete;

	//bool equals(Node<Token>* root) const;
	void simplifyLiterals(ASTnode** root);
	void simplifyZeroMul(ASTnode** root);
	void simplifyZeroAdd(ASTnode** root);
	void simplifyOneMul(ASTnode** root);

	ASTnode* m_root;
};





#endif


