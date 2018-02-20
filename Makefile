CC=gcc
CDEBUG = -g -O0
CPPFLAGS = $(CDEBUG) -I.
LDFLAGS=-g
LIBS = -lstdc++ -lfl
DEPS=list.h common.h element.h execution_context.h machine.h parser.h instructions.h 

SRC		= main.cpp list.cpp execution_context.cpp element.cpp machine.cpp parser.cpp instructions.cpp fsh.tab.cpp lex.yy.cpp

fsh.tab.o : fsh.y
	bison fsh.y
	mv fsh.tab.c fsh.tab.cpp
	$(CC) $(CPPFLAGS) fsh.tab.cpp -o fsh.tab.o

lex.yy.o : fsh.l
	flex fsh.l
	mv lex.yy.c lex.yy.cpp
	$(CC) $(CPPFLAGS) lex.yy.cpp -o lex.yy.o

OBJS	= $(SRC:.cpp=.o) lex.yy.o fsh.tab.o

fsh : $(OBJS) $(DEPS)
	$(CC) $(CPPFLAGS) -o $@ $(OBJS) $(LIBS)

clean: 
	rm $(OBJS) lex.yy.cpp fsh.tab.h fsh.tab.cpp fsh

