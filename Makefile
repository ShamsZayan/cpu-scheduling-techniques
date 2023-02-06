
cc = gcc -g
CC = g++ -g

all:command.o

command.o: lab4-os.cpp
	$(CC)  -o lab4 lab4-os.cpp

