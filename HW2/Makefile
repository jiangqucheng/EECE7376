# Define the executable target
TARGETS = pr1 pr2 pr3
# TARGETS = pr1 pr2
HWID = 2

# Main rule
all: $(TARGETS)

## Rule for building the executable
pr%: hw$(HWID)pr%.c
	gcc $< -o $@

# Rule for cleaning up generated files
clean:
	rm -f $(TARGETS)
	-killall $(TARGETS) > /dev/null 2>&1



