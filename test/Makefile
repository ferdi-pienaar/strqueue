CPPUTEST_DIR = ../../CppUTest3.3
CXXFLAGS = -Wall -g -I .. -I $(CPPUTEST_DIR)/include
#CXXFLAGS += -DDEBUG_PRINT
#CXXFLAGS += -save-temps
VPATH = $(CPPUTEST_DIR)/lib

OBJS = strqueue_test.o

# Execute test, only if executable is new -- the empty tag file timestamps the last time the test ran
testTag: strqueue_test
	./strqueue_test
	touch testTag
	
strqueue_test: $(OBJS) -lCppUTest
	$(CXX) $(CXXFLAGS) $^ -o $@

strqueue_test.o: strqueue_test.cpp ../strqueue.h

.PHONY: clean
clean:
	rm -f ../*o
	rm -f *o

