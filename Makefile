CC=gcc
CDEBUG = -g -O0
CPPFLAGS = $(CDEBUG) -I.
LDFLAGS=-g
LIBS = -lstdc++ -lfl
DEPS=common.h element.h execution_context.h machine.h instructions.h builtins.h fsh.y fsh.l

SRC	= main.cpp execution_context.cpp element.cpp machine.cpp instructions.cpp fsh.tab.cpp lex.yy.cpp print_cmd.cpp err_cmd.cpp io_cmd.cpp builtins.cpp string_cmd.cpp list_cmd.cpp

fsh.tab.o : fsh.y
	bison -d fsh.y
	mv fsh.tab.c fsh.tab.cpp
	$(CC) -c $(CPPFLAGS) fsh.tab.cpp 

lex.yy.o : fsh.l
	flex fsh.l
	mv lex.yy.c lex.yy.cpp
	$(CC) -c $(CPPFLAGS) lex.yy.cpp

OBJS	= $(SRC:.cpp=.o) 

fsh : $(OBJS) $(DEPS)
	$(CC) $(CPPFLAGS) -o $@ $(OBJS) $(LIBS)

clean: 
	rm $(OBJS) lex.yy.cpp fsh.tab.h fsh.tab.cpp fsh

