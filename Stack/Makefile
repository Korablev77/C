CC      = g++
CFLAGS  = -c -g -pedantic -Wall
LDFLAGS = 
OBJFILES = StackElement.o Stack.o Logger.o StackTests.o
SOURCES = 

TARGET  = StackTests

all: $(TARGET) 

$(TARGET): $(OBJFILES)
	$(CC) $(OBJFILES) -o $@

.c.o: 
	$(CC) $(CFLAGS) $< -o $@
 
clean:
	rm -rf *.o