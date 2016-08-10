#Variable definition
CC  = gcc 
CCFLAGS = -Wall 

#rules definition
all : main

main : c-sim.c c-sim.h
	$(CC) $(CCFLAGS) -o c-sim c-sim.c

#This rule cleans up executable file
clean: 
	rm -f main
