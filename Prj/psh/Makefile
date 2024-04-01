
# Define the compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Define the output binary
TARGET = $(BINDIR)/psh

# Object files
OBJS = $(OBJDIR)/main.o $(OBJDIR)/shell.o

# The first rule is the one executed when no parameters are fed to the Makefile
all: directories $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

directories:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)