main:
	@gcc -w -g main.c \
	command.c \
	executeCommand.c \
	handleInput.c \
	initHome.c \
	pastevents.c \
	peek.c \
	proclore.c \
	prompt.c \
	warp.c \
	seek.c \
	wysh.c

run:
	@gcc -o wysh -g *.c && ./wysh

clean:
	rm wysh pastevents.txt a.out