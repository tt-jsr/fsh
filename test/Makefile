CC=gcc
CDEBUG = -g -O0
CPPFLAGS = $(CDEBUG) -I. -I.. 
LDFLAGS=-g
LIBS = -lstdc++ -lcppunit -lreadline
DEPS=../common.h ../element.h ../execution_context.h ../machine.h ../ast.h ../builtins.h ../fsh.y ../fsh.l ../bytecode.h

SRC	= test1.cpp main.cpp ../execution_context.cpp ../element.cpp ../machine.cpp ../ast.cpp ../fsh.tab.cpp ../lex.yy.cpp ../err_cmd.cpp ../io_cmd.cpp ../builtins.cpp ../string_cmd.cpp ../list_cmd.cpp ../map_cmd.cpp ../shell.cpp ../bytecode.cpp ../oclog_cmd.cpp

OBJS	= $(SRC:.cpp=.o)

fsh.tab.o : fsh.y
	bison -d fsh.y
	mv fsh.tab.c fsh.tab.cpp
	$(CC) -c $(CPPFLAGS) fsh.tab.cpp 

lex.yy.o : fsh.l
	flex fsh.l
	mv lex.yy.c lex.yy.cpp
	$(CC) -c $(CPPFLAGS) lex.yy.cpp

test : $(OBJS) $(DEPS)
	$(CC) $(CPPFLAGS) -o $@ $(OBJS) $(LIBS)


clean: 
	rm $(OBJS) lex.yy.cpp fsh.tab.h fsh.tab.cpp test



