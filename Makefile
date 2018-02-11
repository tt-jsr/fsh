CC=gcc
CDEBUG = -g
CFLAGS = $(CDEBUG) -I.
LDFLAGS=-g
LIBS = -lstdc++
DEPS=list.h common.h element.h execution_context.h machine.h parser.h instructions.h 

SRC	= main.cpp list.cpp execution_context.cpp element.cpp machine.cpp parser.cpp instructions.cpp
OBJS = $(SRC:.c=.o)

fsh : $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

clean: 
	rm -r $(OBJS) fsh

