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
  explicit Stack(const int initSize);
  ~Stack();

  bool isOk() const;
  bool dump(TraverseDirection direction = DOWNWARD, FILE* fileToDump = NULL) const;

  bool          push(StackElement  *element);
  StackElement* pop();
  StackElement* top() const;
  size_t        getCurrentSize() const;
  size_t        getSize() const;
  bool          isEmpty() const;

  const static size_t DEFAULT_STACK_SIZE = 1;
private:
  Stack(const Stack &);
  const Stack& operator=(const Stack &);
  //static StackLogger logger;
  //std::unique_ptr< std::shared_ptr<StackElement*> > m_data;
  StackElement **m_data;
  size_t m_size;
  size_t m_currentPosition;
};

#endif

