CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
TARGET = npuzzle
SRCDIR = .
OBJDIR = obj

# Source files
SOURCES = main.c npuzzle_core.c heuristic.c astar.c display.c
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)
HEADERS = npuzzle.h

# Default target
all: $(TARGET)

# Create object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Build target executable
$(TARGET): $(OBJDIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) -lm

# Compile source files to object files
$(OBJDIR)/%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(TARGET) output.txt

# Clean and rebuild
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug build with additional flags
debug: CFLAGS += -DDEBUG -g3 -O0
debug: clean $(TARGET)

# Release build with optimizations
release: CFLAGS += -DNDEBUG -O3
release: clean $(TARGET)

.PHONY: all clean rebuild run debug release