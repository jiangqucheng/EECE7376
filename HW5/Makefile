# Define the executable target
TARGETS = h5-p1-nolock h5-p1-onelock h5-p1-twolocks h5-p1-twolocks-nodummy h5-p2 main
DEBUG = 0  # 0 - nodebug, 1 - debug

RUN_Q2_COUNT ?= 10000
RUN_Q2_THREAD ?= 5

# Main rule
all: $(TARGETS)

## Rule for building the executable 
h5-p%: h5-p%.c
	gcc $< -o $@ -lpthread -DDEBUG=$(DEBUG)

main: h5-p2
	cp $< $@

run0: h5-p1-nolock
	./$<
run1: h5-p1-onelock
	./$<
run2: h5-p1-twolocks
	./$<
run2n: h5-p1-twolocks-nodummy
	./$<

runq2: main
	./$< $(RUN_Q2_COUNT) $(RUN_Q2_THREAD)

test-p1:
	echo "\n" >> results-$@.log 2>&1
	bash -c 'for i in {1..10}; do echo "[[`date +"%Y-%m-%d %H:%M:%S %Z"`]] - SEQ:$$i :" >> results-$@.log; make run1 >> results-$@.log; echo "----------" >> results-$@.log; make run2 >> results-$@.log; echo "----------" >> results-$@.log; done'
	echo "\n" >> results-$@.log 2>&1

make:
	echo "\n" >> results-$@.log 2>&1
	bash -c 'for i in {1..1000}; do echo "[[`date +"%Y-%m-%d %H:%M:%S %Z"`]] - SEQ:$$i :" >> results-$@.log; make runq2 RUN_Q2_THREAD=$$i >> results-$@.log; echo "----------" >> results-$@.log; done'
	bash -c 'for i in 1000 10000 2000 20000 3000 7000 9000; do echo "[[`date +"%Y-%m-%d %H:%M:%S %Z"`]] - SEQ:$$i :" >> results-$@.log; make runq2 RUN_Q2_THREAD=$$i >> results-$@.log; echo "----------" >> results-$@.log; done'
	bash -c 'for i in 1111 11111 2222 22222 3333 7777 9999; do echo "[[`date +"%Y-%m-%d %H:%M:%S %Z"`]] - SEQ:$$i :" >> results-$@.log; make runq2 RUN_Q2_THREAD=$$i >> results-$@.log; echo "----------" >> results-$@.log; done'
	bash -c 'for i in {1..1000}; do echo "[[`date +"%Y-%m-%d %H:%M:%S %Z"`]] - SEQ:$$i :" >> results-$@.log; make runq2 RUN_Q2_COUNT=$$i >> results-$@.log; echo "----------" >> results-$@.log; done'
	echo "\n" >> results-$@.log 2>&1

# Rule for cleaning up generated files
clean:
	rm -f $(TARGETS)
	rm -f results-*.log
	-killall $(TARGETS) > /dev/null 2>&1



