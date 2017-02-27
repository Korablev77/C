#include <iostream>
#include <stddef.h>
#include <vector>
#include "helpers.h"
#include "Stack.h"





Stack::Stack() : m_size(DEFAULT_STACK_SIZE), m_currentPosition(0) {
  try {
    m_data = new StackElement*[m_size];
  } catch (std::bad_alloc &ex) {
    std::cerr << "Stack constructor failed. New threw exception. Don't use stack.\n";
    std::cerr << ex.what() << std::endl;
    m_data = NULL;
    m_size = 0;
  }
} 

Stack::Stack(int initSize) :  m_currentPosition(0) {  
  try {
    if (initSize < 1) {
      std::cerr << "Attempt at creatinng stack with zero or negatives size\n";
      std::cerr << "Stack will be created with default size: " << DEFAULT_STACK_SIZE << std::endl;
      m_size = DEFAULT_STACK_SIZE;
    } else {
      m_size = initSize;
    }

    m_data = new StackElement*[m_size];
  } catch (std::bad_alloc &ex) {
      std::cerr << "Stack constructor failed. New threw exception. Don't use stack.\n";
      std::cerr << ex.what() << std::endl; 
      m_data = NULL;
      m_size = 0;
  }
}

Stack::~Stack() {
  isOk();
  for (int i = 0; i < m_currentPosition; ++i) {
    m_data[i] = NULL;
  }
  delete[] m_data;
  m_data = NULL; 
  m_size = 0;
  m_currentPosition = 0;
}

bool Stack::isOk() const {
  ASSERT(m_data);
  ASSERT(m_currentPosition >= 0);
  ASSERT(m_size > 0);
  ASSERT(m_currentPosition < m_size+1);

  for (int i = 0; i < m_currentPosition; ++i) {
    ASSERT(m_data[i]);
  }

  return true;
}

bool Stack::isEmpty() const {
  if (m_currentPosition > 0) return false;
  return true;
}

bool Stack::dump(TraverseDirection direction) const {
  isOk();
  switch(direction) {
    case DOWNWARD:
      std::cout << "STACK FROM TOP TO BOTTOM:\n";
      for (int i = m_currentPosition-1; i > -1; --i) {
        std::cout << i << ". ";
        m_data[i]->dump();
      }
      break;

    case UPWARD:
      std::cout << "STACK: \n";
      for (int i = 0; i < m_currentPosition; ++i) {
        std::cout << i << ". ";
        m_data[i]->dump();
      }
      break;
  }
  return true;
}

bool Stack::push(StackElement *element) {
  isOk();

  if (m_currentPosition > m_size-1) {

    m_size <<= 1;
    
    try {
      std::cerr << "Reallocating memory\n";
      StackElement **newDataPtr = new StackElement*[m_size];
      for (int i = 0; i < m_currentPosition; ++i) {
        newDataPtr[i] = m_data[i];
      }
      delete[] m_data;
      m_data = newDataPtr;
      newDataPtr = NULL;

    } catch (std::bad_alloc &ex) {
      std::cerr << "Push failed. New threw exception.\n";
      std::cerr << ex.what() << std::endl;
      isOk();
      return false;
    }
  }

  m_data[m_currentPosition++] = element;
  isOk();
  return true;
}

StackElement* Stack::pop() {
  isOk();
  if (isEmpty()) {
    std::cerr << "Pop failed: stack is empty.\n";
    return NULL;
  }
 
  return m_data[--m_currentPosition];
}

size_t Stack::getCurrentSize() const {
  isOk();
  return m_currentPosition;
}

size_t Stack::getSize() const {
  isOk();
  return m_size;
}

StackElement* Stack::top() const {
  isOk();
  if (isEmpty()) {
    std::cerr << "Top failed: stack is empty.\n";
    return NULL;
  }
  return m_data[m_currentPosition-1];
}

/*int main()
{
    // void f(int x);
    // void F(Stack s);
    // f(5);
    Stack s(1);
    //StackElement* t1 = new StackElement(20);
    DoubleElement t2(3);
    StringElement t3("Dksajbfl");
    DoubleElement t4(5.3);
    //StackElement t2(30);
    //t1.dump();
    //s.push(&t1);
    s.push(&t2);
    s.push(&t3);
    s.push(&t4);
    //s.push(t4);
    t4.setValue(2);
    s.dump();
    s.pop();
    s.dump();
    //Stack s(20);
    return 0;
}*/

