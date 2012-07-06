CXXFLAGS = -Wall -g -I .. -I ../../CppUnitLite
#CXXFLAGS += -DDEBUG_PRINT
#CXXFLAGS += -save-temps
VPATH = ../../CppUnitLite

OBJS = strqueue_test.o

# Execute test, only if executable is new -- the empty tag file timestamps the last time the test ran
testTag: strqueue_test
	./strqueue_test
	touch testTag
	
strqueue_test: $(OBJS) -lCppUnitLite
	$(CXX) $(CXXFLAGS) $^ -o $@

strqueue_test.o: strqueue_test.cpp ../strqueue.h

.PHONY: clean
clean:
	rm -f ../*o
	rm -f *o

