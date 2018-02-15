CC=gcc
CDEBUG = -g -O0
CPPFLAGS = $(CDEBUG) -I. -I.. 
LDFLAGS=-g
LIBS = -lstdc++ -lcppunit
DEPS=parser_test.h ../list.h ../common.h ../element.h ../execution_context.h ../machine.h ../parser.h ../instructions.h 

SRC		= main.cpp parser_test.cpp ../parser.cpp ../instructions.cpp ../machine.cpp ../element.cpp
OBJS	= $(SRC:.cpp=.o)

test : $(OBJS) $(DEPS)
	$(CC) -o $@ $(OBJS) $(LIBS)

clean: 
	rm $(OBJS) test

