#include <exception>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>
#include "Stack.h"
#include "StackElement.h"



class TestNotPassedException : public std::runtime_error {
public:
  explicit TestNotPassedException(const std::string& msg) : std::runtime_error(msg.c_str()) {};
  explicit TestNotPassedException(const char* msg) : std::runtime_error(msg) {}

};



#define TEST(testName) void testName ## Test() ; \
  void testName() { \
    std::cerr << "Running test " #testName ": "; \
    try { \
      testName ## Test(); \
      std::cerr << "... passed\n"; \
    } catch(...) { \
      std::cerr << "... FAILED\n"; \
      throw; \
    } \
  } \
  void testName ## Test()


#define ASSERT_EQUAL(first, second) do { \
  if (first != second) { \
    std::ostringstream oss; \
    oss << "Required equal failed: " << #first << " != " << #second << " <==> " << first << " != " << second << "\n" \
        << "TEST: " << __FUNCTION__ << " At line: " << __LINE__ << "\n"; \
    throw TestNotPassedException(oss.str()); \
  }\
}while (false)

//#define ASSERT_TRUE(cond) do { \
//  if(())
//}



TEST(StackCreateDefaultConstructor) {
  Stack s;
  ASSERT_EQUAL(s.getSize(), s.DEFAULT_STACK_SIZE);
  ASSERT_EQUAL(s.getCurrentSize(), 0);

  Stack *s1 = new Stack();
  ASSERT_EQUAL(s1->getSize(), s1->DEFAULT_STACK_SIZE);
  ASSERT_EQUAL(s1->getCurrentSize(), 0);
  delete s1;

}


TEST(StackCreateArgConstructor) {
  Stack s(-56);
  ASSERT_EQUAL(s.getSize(), s.DEFAULT_STACK_SIZE);
  ASSERT_EQUAL(s.getCurrentSize(), 0);

  Stack s1(std::numeric_limits<int>::max()); //max_int
  ASSERT_EQUAL(s1.getSize(), std::numeric_limits<int>::max());
  ASSERT_EQUAL(s1.getCurrentSize(), 0);

  Stack *s2 = new Stack(20);
  ASSERT_EQUAL(s2->getSize(), 20);
  ASSERT_EQUAL(s2->getCurrentSize(), 0);
  delete s2;
}

TEST(StackPush) {
  Stack *s = new Stack(20);

  DoubleElement *d1 = new DoubleElement(66.6);
  DoubleElement d2(77.7);

  StringElement *str1 = new StringElement(std::string("hello"));
  StringElement str2("world");

  s->dump();

  s->push(d1);
  s->push(&d2);
  s->push(str1);
  s->push(&str2);


  ASSERT_EQUAL(s->getCurrentSize(), 4);
  ASSERT_EQUAL((*(StringElement*)s->top()).getValue(), str2.getValue());

  delete d1;
  delete str1;
}


TEST(StackRellocatingMemory) {
 Stack *s = new Stack(1);
 ASSERT_EQUAL(s->getSize(), 1);


  DoubleElement *d1 = new DoubleElement(66.6);
  DoubleElement d2(77.7);

  StringElement *str1 = new StringElement(std::string("hello"));
  StringElement str2("world");

  s->push(d1);
  s->push(&d2);

  ASSERT_EQUAL(s->getSize(), 2);

  s->push(str1);
  s->push(&str2);

  ASSERT_EQUAL(s->getSize(), 4);

  s->push(str1);

  ASSERT_EQUAL(s->getSize(), 8);

  delete d1;
  delete str1;

}

TEST(StackPushPop) {
  Stack *s = new Stack(1);

  DoubleElement *d1 = new DoubleElement(66.6);
  DoubleElement d2(77.7);

  StringElement *str1 = new StringElement(std::string("hello"));
  StringElement str2("world");

  s->push(d1);
  s->push(&d2);
  s->push(str1);
  s->push(&str2);

  ASSERT_EQUAL(s->getCurrentSize(), 4);
  ASSERT_EQUAL((*(StringElement*)s->pop()).getValue(), "world");
  ASSERT_EQUAL((*(StringElement*)s->pop()).getValue(), "hello");
  ASSERT_EQUAL((*(DoubleElement*)s->pop()).getValue(), d2.getValue());
  ASSERT_EQUAL((*(DoubleElement*)s->pop()).getValue(), d1->getValue());

  ASSERT_EQUAL(s->getCurrentSize(), 0);

  delete d1;
  delete str1;
}


TEST(StackPopTopFromEmptyStack) {
  Stack *s = new Stack(1);

  DoubleElement *d1 = new DoubleElement(66.6);
  DoubleElement d2(77.7);

  s->push(d1);
  s->push(&d2);

  s->pop();
  s->pop();

  ASSERT_EQUAL(s->getCurrentSize(), 0);
  ASSERT_EQUAL(s->pop(), NULL);
  ASSERT_EQUAL(s->pop(), NULL);
  ASSERT_EQUAL(s->top(), NULL);
  ASSERT_EQUAL(s->top(), NULL);

  s->push(d1);

  ASSERT_EQUAL((*(DoubleElement*)s->top()).getValue(), d1->getValue());
  ASSERT_EQUAL((*(DoubleElement*)s->pop()).getValue(), d1->getValue()); 

  ASSERT_EQUAL(s->pop(), NULL);
  ASSERT_EQUAL(s->top(), NULL);

  delete d1;
}




int runTests() {
  try {

    StackCreateDefaultConstructor();
    StackCreateArgConstructor();
    StackRellocatingMemory();
    StackPush();
    StackPushPopTest();
    StackPopTopFromEmptyStack();

  } catch (std::exception& e) {
    std::cerr << "Tests failed\n";
    std::cerr << e.what();
    return -1;
  }
  std::cerr << "All tests are passed\n";
  return 0;
}


#define TEST_MODE

int main() {
  #ifdef TEST_MODE
    std::cerr << "RUNNING TESTS: " << std::endl;
    return runTests();
  #endif

    return 0;
}
