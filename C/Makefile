# The compiler to use
CC = gcc

# Compiler flags (Enable warnings and debugging)
CFLAGS = -Wall -g

# The name of the executable
TARGET = my_shell

all: $(TARGET)

$(TARGET): shell.c
	$(CC) $(CFLAGS) -o $(TARGET) shell.c

clean:
	rm -f $(TARGET)
