#include "StackElement.h"


StackElement::StackElement() {}
StackElement::~StackElement() {}

DoubleElement::DoubleElement(double initValue) : 
	m_value(initValue) {};

DoubleElement::~DoubleElement() {}

double DoubleElement::getValue() const {
  return m_value;
}

void DoubleElement::setValue(const double newValue) {
  m_value = newValue;
}

void DoubleElement::dump() const {
  fprintf(stdout, "m_value = %lg \n", m_value);
}


StringElement::StringElement(std::string initString) : 
	m_string(initString){};

StringElement::StringElement(const char* initString) : 
	m_string(initString){};

std::string StringElement::getValue() const {
  return m_string;
}

void StringElement::setValue(const std::string newStr) {
  m_string = newStr;
}

void StringElement::dump() const {
  fprintf(stdout, "m_string = %s \n", m_string.c_str());
}

StringElement::~StringElement() {}

