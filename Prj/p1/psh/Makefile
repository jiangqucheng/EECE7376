
# Define the compiler
CXX = g++

# Compiler flags
DEBUG = 0
CXXFLAGS = -Wall -g 
ifeq ($(DEBUG), 1)
CXXFLAGS += -DDEBUG
endif

# Directories
INCDIR = inc
SRCDIR = src
UNITTEST_SRCDIR = unittest
OBJDIR = obj
BINDIR = bin

# Define the output binary
TARGET = $(BINDIR)/psh $(BINDIR)/unittest_main_ast

# Source files
COMPONENT_SOURCES = $(shell find $(SRCDIR) -type f -name '*.cpp' | grep -v 'main.cpp')
SOURCES := $(COMPONENT_SOURCES) $(SRCDIR)/main.cpp

# Object files
COMPONENT_OBJS = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(COMPONENT_SOURCES:.cpp=.o))
OBJS = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.cpp=.o))

# The first rule is the one executed when no parameters are fed to the Makefile
all: $(TARGET)

# Rules to build executable
$(BINDIR)/psh: $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BINDIR)/unittest_main_%: $(OBJDIR)/unittest_main_%.o $(COMPONENT_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rules to build object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(OBJDIR)/%.o: $(UNITTEST_SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

run: $(BINDIR)/psh
	$<

directories:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)
