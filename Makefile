all:
	gcc -o test.out test.c decoder.c input.c
clean:
	rm -f *.out
