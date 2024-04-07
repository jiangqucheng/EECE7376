
# Define the compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g 

# Define the output binary
TARGET = ast

# Object files
# OBJS = $(OBJDIR)/main.o $(OBJDIR)/shell.o

# The first rule is the one executed when no parameters are fed to the Makefile
all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean: 
	rm -rf $(TARGET)