CC = gcc
CFLAGS = -Iopcodes -Wall -Wextra -g -Wno-sign-compare -Wno-unused-parameter
SRC = IsaSim.c
TARGET = IsaSim

all: $(TARGET)
	./$(TARGET) tests/task3/width.bin 
	./a.out tests/task3/width.res
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
