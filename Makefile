all:
	gcc -Wall *.c

debug:
	gcc -Wall -g *.c

dll:
	gcc -shared -o $(name).dll $(name).c