CC = gcc
CFLAGS = -Iopcodes -Wall -Wextra -g -Wno-sign-compare -Wno-unused-parameter
SRC = IsaSim.c
TARGET = IsaSim

all: $(TARGET)
	./$(TARGET) tests/task3/loop.bin 

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
