#include <stdio.h>
#include <string>


#include "include/Interpreter.hpp"


#ifdef VERSION__
#undef VERSION__
#endif
#define VERSION__ 0.1

static void greetings() {
	printf("%s\n", "Welcome to Interactive Mathematical Interpreter v.0.1!");
	printf("%s\n", "To enter test mode input \"test\"");
	printf("%s\n", "To quit from Interpreter press q");
	printf("%s\n", "To get help enter \"help\"");
	printf("%s\n", "To print current symbol talbe inputr \"print\"");
	printf("%s\n", "Input and derive any expression!");
}

static void help() {
	printf("%s\n", "Author: Nikita Pettik");
	printf("%s %f\n", "Version: ", VERSION__);
}

static bool runTests() {
	char command[64];
	FILE* sourceCode = fopen("source_code.txt", "r");
	ASSERT(nullptr != sourceCode);
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
	//tree->simplify();
	tree->simplify();
	tree->dump(nullptr, true);
	printf("%s\n", tree->toString().c_str());
	delete tree;
}

static void flushInput() {
	int ch;
	while ((ch = getchar()) != EOF && ch != '\n');
}


Interpreter::Interpreter() {}


//грис - приведение подобных
//TODO: type checking

bool Interpreter::run(Mode mode /* = TEST_MODE*/) {
	
	//if (TEST_MODE == mode) return runTests();

	greetings();

	for(;;) {	

		try {
			for (int i = 0; i < 64; i++) m_command[i] = '\0';
			if (feof(stdin)) flushInput();
			if (1 != scanf("%[^\n]%*c", m_command)) {
				printf("%s\n", "Scanf failed to read input expression. Please, try again.");
				flushInput();
				continue;
			}

			if (0 == strcmp(m_command, "q")) {
				TokenHolder::undef();
				return true;
			} else if (0 == strcmp(m_command, "test")) runTests();
			else if (0 == strcmp(m_command, "help")) help();
			else if (0 == strcmp(m_command, "print")) TokenHolder::printSymbolTable();
			else if (0 == strcmp(m_command, "undef")) TokenHolder::undef(); 
			else {
				for (int i = 0; i < 32; i++) printf("%i", m_command[i]);
				for (int i = 0; i < 32; i++) {
					printf("%c", m_command[i]);
				}
				printf("Input expression: %s\n", m_command);
				TokenHolder* holder = m_lexer.tokenize(m_command);
				m_parser.setTokenHolder(holder);

				ASTree *tree = m_parser.buildAST();
				ASSERT((nullptr != tree));
				tree->dump(nullptr, true);
				printf("%s\n", tree->toString().c_str());
				delete tree;
			}

		} catch(...) {
			exceptionDispatcher();
			printf("%s\n", "Try again!");
		}
	}



	return false;
}


