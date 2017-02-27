#ifndef STACKELEMENT_H
#define STACKELEMENT_H

#include <string>
#include <iostream>


//simple interface for stack elements
class StackElement {
public:
  StackElement(){};

  virtual void dump() const = 0;
  virtual ~StackElement(){};
};





class DoubleElement : public StackElement  {
public:
  explicit DoubleElement(double initValue) : m_value(initValue){};
  ~DoubleElement(){};

  double getValue() const;
  void setValue(const double newValue);
  void dump() const;
private:
  double m_value;
};

double DoubleElement::getValue() const {
  return m_value;
}

void DoubleElement::setValue(const double newValue) {
  m_value = newValue;
}

void DoubleElement::dump() const {
  std::cout << "[m_value: " << m_value << "]\n";
}


class StringElement : public StackElement {
public:
  explicit StringElement(std::string initString) : m_string(initString){};
  explicit StringElement(const char* initString) : m_string(initString){};

  std::string getValue() const;
  void setValue(const std::string newStr);
  void dump() const;
private:
  std::string m_string;
};

std::string StringElement::getValue() const {
  return m_string;
}

void StringElement::setValue(const std::string newStr) {
  m_string = newStr;
}

void StringElement::dump() const {
  std::cout << "[m_string: " <<m_string << "]\n";
}


#endif
