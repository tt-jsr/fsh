CC=gcc
CFLAGS=-g
DEPS=list.h common.h element.h execution_context.h for_cmd.h

fsh : main.o list.o execution_context.o element.o for_cmd.o $(DEPS)
	$(CC) $(CFLAGS) main.o list.o execution_context.o element.o for_cmd.o -o fsh -lstdc++

list.o : list.cpp $(DEPS)
	$(CC) $(CFLAGS) -c list.cpp -o list.o

execution_context.o : execution_context.cpp $(DEPS)
	$(CC) $(CFLAGS) -c execution_context.cpp -o execution_context.o

element.o : element.cpp $(DEPS)
	$(CC) $(CFLAGS) -c element.cpp -o element.o

main.o: main.cpp $(DEPS)
	$(CC) $(CFLAGS) -c main.cpp -o main.o

for_cmd.o: for_cmd.cpp $(DEPS)
	$(CC) $(CFLAGS) -c for_cmd.cpp -o for_cmd.o

clean: 
	rm -r list.o main.o execution_context.o element.o for_cmd.o fsh

