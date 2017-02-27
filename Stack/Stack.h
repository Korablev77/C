#ifndef STACK_H
#define STACK_H

#include "StackElement.h"

typedef enum {
  UPWARD = 1,
  DOWNWARD
}TraverseDirection;


class Stack {
public:
  Stack();
  explicit Stack(int initSize);
  ~Stack();

  bool isOk() const;
  bool dump(TraverseDirection direction = DOWNWARD) const;

  bool          push(StackElement* element);
  StackElement* pop();
  StackElement* top() const;
  size_t        getCurrentSize() const;
  size_t        getSize() const;
  bool          isEmpty() const;

  const static int DEFAULT_STACK_SIZE = 1;
private:
  //std::unique_ptr< std::shared_ptr<StackElement*> > m_data;
  StackElement **m_data;
  size_t m_size;
  size_t m_currentPosition;
};

#endif

