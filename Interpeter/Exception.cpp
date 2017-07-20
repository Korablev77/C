#include "include/Exception.hpp"


const char* BaseException::what() const _NOEXCEPT {
	return std::runtime_error::what();
}


InternalTranslationException::InternalTranslationException(std::string msg, Token* token) : 
		BaseException(msg + "\n-->Token: "+token->toString())  {}


const char* InternalTranslationException::what() const throw() {
	return BaseException::what();
}


void exceptionDispatcher() {
	try{
		throw;
	} catch(const InternalTranslationException &e) {
		fprintf(stderr, "%s\n", e.what());
	} catch(const BaseException &e) {
		fprintf(stderr, "%s\n", e.what());
	} catch(...) {
		fprintf(stderr, "%s\n", "Unknown exception has been handled in main.\n");
		fprintf(stderr, "%s\n", "Unexpected shutdown.\n");
		fprintf(stderr, "%s\n", "Creating log...\n");
		//TODO: add crash dump
	}
}


