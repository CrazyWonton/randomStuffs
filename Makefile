test : simCirc
	./simCirc ANDORMUX 010
	
test2 : simCirc
	./simCirc MUX4 011011

gdb : simCirc
	gdb -ex "break main" -ex "run ANDORMUX 010" simCirc

simCirc : simCirc.c circuit.c circuit.h
	gcc -Wall -g -o simCirc simCirc.c circuit.c