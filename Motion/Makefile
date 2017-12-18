CC      = gcc
CFLAGS  = -c -g -pedantic -Wall -Winvalid-pch -O2
LDFLAGS = 
TARGET  = brownian_motion

all: $(TARGET)

$(TARGET): brownian_motion.o
	$(CC) brownian_motion.o -o $(TARGET)

brownian_motion.o: brownian_motion.c
	$(CC) $(CFLAGS) brownian_motion.c
clean:
	echo "Cleaning..."
	rm -rf *.o
