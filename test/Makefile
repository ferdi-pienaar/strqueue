CXXFLAGS = -Wall -g -I .. -I ../../CppUnitLite #  -DDEBUG_PRINT  -save-temps
VPATH = ../../CppUnitLite

OBJS = strqueue_test.o

all: strqueue_test

strqueue_test: $(OBJS) -lCppUnitLite
	$(CXX) $(CXXFLAGS) $^ -o $@
	./strqueue_test

strqueue_test.o: strqueue_test.cpp ../strqueue.h

.PHONY: clean
clean:
	rm -f ../*o
	rm -f *o

