CC=gcc
CFLAGS=-g
DEPS=list.h common.h element.h execution_context.h machine.h parser.h instructions.h 

fsh : main.o list.o execution_context.o element.o machine.o parser.o instructions.o $(DEPS)
	$(CC) $(CFLAGS) main.o list.o execution_context.o element.o machine.o parser.o instructions.o -o fsh -lstdc++

list.o : list.cpp $(DEPS)
	$(CC) $(CFLAGS) -c list.cpp -o list.o

execution_context.o : execution_context.cpp $(DEPS)
	$(CC) $(CFLAGS) -c execution_context.cpp -o execution_context.o

element.o : element.cpp $(DEPS)
	$(CC) $(CFLAGS) -c element.cpp -o element.o

main.o: main.cpp $(DEPS)
	$(CC) $(CFLAGS) -c main.cpp -o main.o

machine.o: machine.cpp $(DEPS)
	$(CC) $(CFLAGS) -c machine.cpp -o machine.o

parser.o: parser.cpp $(DEPS)
	$(CC) $(CFLAGS) -c parser.cpp -o parser.o

instructions.o: instructions.cpp $(DEPS)
	$(CC) $(CFLAGS) -c instructions.cpp -o instructions.o

clean: 
	rm -r list.o main.o execution_context.o element.o for_cmd.o machine.o parser.o instructions.o fsh

