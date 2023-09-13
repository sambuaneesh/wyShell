main:
	@gcc -o wysh -g *.c

run:
	@gcc -o wysh -g *.c && ./wysh

clean:
	rm wysh pastevents.txt a.out