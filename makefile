main:
	gcc -o whysh -g *.c

run:
	gcc -o whysh -g *.c && ./whysh

clean:
	rm whysh pastevents.txt