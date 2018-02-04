#ifndef TEST_FRAMEWORK_HPP
#define TEST_FRAMEWORK_HPP


#include <exception>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>

#include "../include/Interpreter.hpp"
#include "../include/Interpreter.hpp"
#include "../include/Interpreter.hpp"



class TestNotPassedException : public std::runtime_error {
public:
  explicit TestNotPassedException(const std::string& msg) : std::runtime_error(msg.c_str()) {};
  explicit TestNotPassedException(const char* msg) : std::runtime_error(msg) {}

};

#define TEST(testName) void testName ## Test() ; \
  void testName() { \
    fprintf(stdout, "%s\n", "Running test " #testName ": "); \
    try { \
      testName ## Test(); \
      fprintf(stdout, "    %s\n", "... passed\n"); \
    } catch(...) { \
      fprintf(stdout, "    %s\n", "... FAILED\n"); \
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
  } \
} while (false)


TEST(BasicTest1) {
  char command[64];
  FILE* testCodeExpr = fopen("test_expr1.txt", "r");
  ASSERT(nullptr != testCodeExpr);
  fscanf(sourceCode, "%[^\n]%*c", command);
  printf("%s\n", command);
  ASSERT(EOF != fclose(sourceCode));

  Lexer lex;
  Parser pars;

  TokenHolder* holder = lex.tokenize(command);
  pars.setTokenHolder(holder);

  ASTree *tree = pars.buildAST();
  ASSERT((nullptr != tree));
  tree->dump(nullptr, true);
  tree->simplify();
  tree->dump(nullptr, true);
  printf("%s\n", tree->toString().c_str());
  delete tree;
}


int runTests() {
  try {

    BasicTest1();


  } catch (std::exception& e) {
    std::cerr << "Tests failed\n";
    std::cerr << e.what();
    return -1;
  }
  std::cerr << "All tests are passed\n";
  return 0;
}

#endif

