all:
	gcc -g main.c pcb.c pcb.h list.o -o pcb

clean:
	rm pcb
