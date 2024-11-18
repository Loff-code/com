CC = gcc
CFLAGS = -Iopcodes -Wall -Wextra -g -Wno-sign-compare -Wno-unused-parameter
SRC = IsaSim.c
TARGET = IsaSim

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
