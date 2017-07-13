#ifndef STACKELEMENT_H
#define STACKELEMENT_H

#include <string>
#include <iostream>


//Simple interface for stack elements
class StackElement {
public:
  StackElement();

  virtual void dump() const = 0;
  virtual ~StackElement();
};



class DoubleElement : public StackElement  {
public:
  explicit DoubleElement(double initValue);
  ~DoubleElement();

  double getValue() const;
  void setValue(const double newValue);
  void dump() const;
private:
  double m_value;
};


class StringElement : public StackElement {
public:
  explicit StringElement(std::string initString);
  explicit StringElement(const char* initString);
  ~StringElement();
  std::string getValue() const;
  void setValue(const std::string newStr);
  void dump() const;
private:
  std::string m_string;
};



#endif
