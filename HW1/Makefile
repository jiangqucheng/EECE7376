# Define the executable target
TARGETS = pr1 pr2 pr3a pr3b pr3c pr4
HWID = 1

# Main rule
all: $(TARGETS)

## Rule for building the executable 
pr%: hw$(HWID)pr%.c
	gcc $< -o $@

# Rule for cleaning up generated files
clean:
	rm -f $(TARGETS)
	-killall $(TARGETS) > /dev/null 2>&1



