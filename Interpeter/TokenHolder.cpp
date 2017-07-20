#include "include/TokenHolder.hpp"


IdValue::IdValue(const double value) : m_value(value), m_type(DOUBLE) {}

IdValue::IdValue(ASTnode* const root) : m_root(root), m_type(TREE) {}

void IdValue::check(IdValueType type) const {
	if (type != m_type) THROW_EXCEPTION(BaseException, 
		"Attempt at using wrong type in union.");
}

double& IdValue::value() {
	check(DOUBLE);
	return m_value;
}

ASTnode*& IdValue::root() {
	check(TREE);
	return m_root;
}

IdValue::IdValueType IdValue::getType() const {
	return m_type;
}


TokenHolder::TokenHolder(const std::vector<Token*> &tokens) : 
	m_tokens(tokens), m_currentPosition(0) {}

Token* TokenHolder::getCurrentToken() const {
	return m_tokens[m_currentPosition];
}

bool TokenHolder::switchToNextToken() {
	if (m_currentPosition < m_tokens.size()-1) {
		m_currentPosition++;
		return true;
	}
	return false;
}

void TokenHolder::addSymbol(const std::string &id, IdValue value) {
	gLogger.log(std::string("Adding new symbol: ") + id);
	std::map<std::string, IdValue>::iterator it = m_symbolTable.find(id);
	if (it != m_symbolTable.end()) {
		if (IdValue::TREE == it->second.getType()) {
			delete it->second.root();
		}
		it->second = value;
	} else {
		m_symbolTable.insert(std::pair<std::string, IdValue>(id, value));
	}
}

IdValue TokenHolder::getSymbolValue(const std::string &id) {
	std::map<std::string, IdValue>::iterator it = m_symbolTable.find(id);
	if (it == m_symbolTable.end()) {
		return IdValue(SYMBOL_NOT_FOUND);
	}
	return it->second;
}

void TokenHolder::printSymbolTable()  {
	std::map<std::string, IdValue>::iterator it;
	fprintf(stdout, "%s\n", "===Symbol Table===");
	for (it = m_symbolTable.begin(); it != m_symbolTable.end(); ++it) {
		if (IdValue::DOUBLE == it->second.getType()) 
			fprintf(stdout, "%s : %lg\n", it->first.c_str(), it->second.value());
		else fprintf(stdout, "%s : %s\n", it->first.c_str(), toStringRecursive(it->second.root()).c_str());
	}
	fprintf(stdout, "%s\n", "===End of symbol table===");
}

void TokenHolder::printTokens() {
	std::vector<Token*>::iterator it;
	gLogger.log("===Token holder dump===");
	for (it = m_tokens.begin(); it != m_tokens.end(); ++it) {
		gLogger.log((*it)->toString());
	}
	gLogger.log("===End of dump===");
}

void TokenHolder::undef(const std::string &id /*= all*/) {
	std::map<std::string, IdValue>::iterator it;
	if ("all" == id) {
		for (it = m_symbolTable.begin(); it != m_symbolTable.end(); ++it) {
			if (IdValue::TREE == it->second.getType()) {
				delete (it->second.root());
			}
		}
		m_symbolTable.clear();
		gLogger.log("Erasing all elements from symbol table.");
	} else {
		it = m_symbolTable.find(id);
		if (it != m_symbolTable.end()) {
			if (IdValue::TREE == it->second.getType()) recursiveDelete(it->second.root());
			gLogger.log(std::string("Element ") + id + 
				std::string(" was erased from symbol table."));
			m_symbolTable.erase(id);
		} else {
			gLogger.log(std::string("Couldn't erase element ") + id + 
				std::string(" : symbol table doesn't contain such element."), WARNING);
		}
	}
}

TokenHolder::~TokenHolder() {
	for (auto it = m_tokens.begin(); it != m_tokens.end(); ++it) {
		delete *it;
	}
}

std::map<std::string, IdValue> TokenHolder::createSymbolTable() {
	return std::map<std::string, IdValue>();
}

double TokenHolder::SYMBOL_NOT_FOUND = -666.666;

std::map<std::string, IdValue> TokenHolder::m_symbolTable = TokenHolder::createSymbolTable();


