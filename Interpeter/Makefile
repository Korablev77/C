CC      = g++ 
CFLAGS  = -c -std=c++11 -g -pedantic -Wall -Winvalid-pch
LDFLAGS = 
TARGET  = interpreter

all: $(TARGET)

$(TARGET): Main.o Token.o Lexer.o Logger.o ASTtree.o Parser.o Interpreter.o Exception.o TokenHolder.o
	$(CC) Main.o Token.o Lexer.o Logger.o ASTtree.o Parser.o Interpreter.o Exception.o TokenHolder.o -o $(TARGET)

Exception.o: Exception.cpp
	$(CC) $(CFLAGS) Exception.cpp

Logger.o: Logger.cpp
	$(CC) $(CFLAGS) Logger.cpp

Token.o: Token.cpp
	$(CC) $(CFLAGS) Token.cpp

Main.o: Main.cpp
	$(CC) $(CFLAGS) Main.cpp

Lexer.o: Lexer.cpp 
	$(CC) $(CFLAGS) Lexer.cpp

ASTtree.o: ASTtree.cpp
	$(CC) $(CFLAGS) ASTtree.cpp

Parser.o: Parser.cpp
	$(CC) $(CFLAGS) Parser.cpp

Interpreter.o: Interpreter.cpp
	$(CC) $(CFLAGS) Interpreter.cpp

TokenHolder.o: TokenHolder.cpp
	$(CC) $(CFLAGS) TokenHolder.cpp


clean:
	rm -rf *.o