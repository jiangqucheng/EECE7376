# Define the executable target
TARGETS = pr2 pr3 files 
# TARGETS = pr1 pr2
HWID = 6

# Main rule
all: $(TARGETS)

## Rule for building the executable
pr%: hw$(HWID)pr%.c
	gcc $< -o $@

files: pr3
	cp $< $@

run: files
	@echo -n "\n"
	-./$< info hello.txt
	@echo -n "\n"
	-./$< info files
	@echo -n "\n"
	-./$< link files files2hd
	@echo -n "\n"
	-./$< symlink files files2sf
	@ls -l -a --color
	@echo -n "\n"
	-./$< rm files2sf
	@echo -n "\n"
	-./$< rm files2hd
	@echo -n "\n"
	-./$< rm hello.txt
	@echo -n "\n"
	@mkdir t_hello
	-./$< rm t_hello
	@ls -l -a --color
	@rm -rf t_hello 

# Rule for cleaning up generated files
clean:
	rm -f $(TARGETS)
	@-killall $(TARGETS) > /dev/null 2>&1 | echo > /dev/null 2>&1 


