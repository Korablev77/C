
#include "include/Interpreter.hpp"
#include "include/Exception.hpp"


int main(int argc, char const *argv[])
{
	Interpreter interpreter;
	try {
		interpreter.run();
	} catch(...) {
		exceptionDispatcher();
	}
	return 0;
}