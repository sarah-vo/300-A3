all:
	gcc -g main.c pcb.c list.o -o pcb

clean:
	rm pcb