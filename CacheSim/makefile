#Variable definition
CC  = gcc 
CCFLAGS = -g -Wall

#rules definition
all : main

main : c-sim.c c-sim.h
	$(CC) $(CCFLAGS) -o c-sim c-sim.c -lm

#This rule cleans up executable file
clean: 
	rm -f main
