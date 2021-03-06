CC=gcc
CDEBUG = -g -O0
CPPFLAGS = $(CDEBUG) -I.
LDFLAGS=-g
LIBS = -lstdc++ -lreadline
DEPS=common.h element.h execution_context.h machine.h builtins.h fsh.y fsh.l shell.h ast.h bytecode.h

SRC	= main.cpp execution_context.cpp element.cpp machine.cpp ast.cpp fsh.tab.cpp lex.yy.cpp err_cmd.cpp io_cmd.cpp builtins.cpp string_cmd.cpp list_cmd.cpp shell.cpp map_cmd.cpp bytecode.cpp

SRC += oclog_cmd.cpp
DEPS += oclog_cmd.h
OBJS	= $(SRC:.cpp=.o) 

fsh : $(OBJS) $(DEPS)
	$(CC) $(CPPFLAGS) -o $@ $(OBJS) $(LIBS)

grammer: fsh.tab.o lex.yy.o

fsh.tab.cpp : fsh.y
	bison -d fsh.y
	mv fsh.tab.c fsh.tab.cpp

lex.yy.cpp : fsh.l
	flex fsh.l
	mv lex.yy.c lex.yy.cpp


clean: 
	rm $(OBJS) lex.yy.cpp fsh.tab.cpp fsh

