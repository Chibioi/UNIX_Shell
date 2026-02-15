# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -g -Isrc/Input_reader/Queue_buffer
LDFLAGS = -lcunit

# Directory Paths
SRC_DIR = src/Input_reader/Queue_buffer
TEST_DIR = src/tests

# Files
# The implementation file
SRC = $(SRC_DIR)/Queue_buffer.c
# The test file
TEST_SRC = $(TEST_DIR)/tests.c
# The output executable name
TARGET = run_tests

# Default rule: Build the test runner
all: $(TARGET)

# Link the objects into the final executable
$(TARGET): $(SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

# Rule to run the tests immediately
test: $(TARGET)
	./$(TARGET)

# Clean up the executable
clean:
	rm -f $(TARGET)

.PHONY: all test clean
