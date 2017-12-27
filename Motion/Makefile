CC      = gcc
CFLAGS  = -c -g -pedantic -Wall -Winvalid-pch -O2 -std=c99 
LDFLAGS = 
TARGET  = brownian_motion

all: $(TARGET)

$(TARGET): brownian_motion.o
	$(CC) brownian_motion.o -o $(TARGET)

brownian_motion.o: brownian_motion.c
	$(CC) $(CFLAGS) brownian_motion.c
.PHONY: clean
clean:
	@echo "Cleaning..."
	rm -rf *.o
