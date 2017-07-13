#include <iostream>
#include <stddef.h>
#include <vector>

#include <unistd.h>
#include <fcntl.h>

#include "Helpers.h"
#include "Stack.h"


typedef enum {
  DATA_POINTER_NULL_ERROR,
  STACK_OVERFLOW
}StackError;


Stack::Stack() : m_size(DEFAULT_STACK_SIZE), m_currentPosition(0) {
  try {
    m_data = new StackElement*[m_size];
  } catch (std::bad_alloc &ex) {
    std::cerr << "Stack constructor failed. Operator new threw exception. Don't use stack.\n";
    std::cerr << ex.what() << std::endl;
    m_data = NULL;
    m_size = 0;
  }
} 

Stack::Stack(const int initSize) :  m_currentPosition(0) {  
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
      std::clog << "Stack constructor failed. New threw exception. Don't use stack.\n";
      std::clog << ex.what() << std::endl; 
      m_data = NULL;
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
  LOG_TRACE();

  if (m_data == NULL) return DATA_POINTER_NULL_ERROR;

  ASSERT(m_data);
  ASSERT(m_currentPosition >= 0);//$debug
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

bool Stack::dump(TraverseDirection direction /* = DOWNWARD*/, FILE* fileToDump /* = NULL*/) const {
  //isOk();

  FILE *logFile = stdout;

  if (fileToDump != NULL && fcntl(fileno(fileToDump), F_GETFL) != -1) {
    logFile = fileToDump;
  }

  if (isEmpty()) {
    fprintf(logFile, "%s \n", "Stack is empty.");;
    return false;
  }

  switch(direction) {
    case DOWNWARD:
      fprintf(logFile, "%s\n", "STACK FROM TOP TO BOTTOM: ");
      fprintf(logFile, "%s\n", "=========================");
      fprintf(logFile, "Stack address: %p \n", this);

      for (int i = m_currentPosition-1; i > -1; --i) {
        fprintf(logFile, "%d. ", i);
        fprintf(logFile, "%p: ", m_data+i);
        m_data[i]->dump();
        fprintf(logFile, "\n");
      }

      fprintf(logFile, "%s\n", "=========================\n");
      break;

    case UPWARD:
      fprintf(logFile, "%s\n", "STACK FROM BOTTOM TO TOP:\n");
      fprintf(logFile, "%s\n", "=========================");
      fprintf(logFile, "Stack address: %p \n", this);

      for (int i = 0; i < m_currentPosition; ++i) {
        fprintf(logFile, "%d. ", i);
        fprintf(logFile, "%p: ", m_data+i);
        m_data[i]->dump();
        fprintf(logFile, "\n");
      }

      fprintf(logFile, "%s\n", "=========================\n");
      break;

    default:
      fprintf(logFile, "%s\n", "Unknown direction");
      break;
  }
  return true;
}

bool Stack::push(StackElement *element) {
  isOk();
  LOG_TRACE();
  //logger.log("PUSH");

  if (m_currentPosition > m_size-1) {

    m_size <<= 1;
    
    try {
      std::clog << "Reallocating memory\n";
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



