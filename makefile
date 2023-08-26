main:
	@gcc -w -o whysh -g main.c \
	command.c \
	executeCommand.c \
	handleInput.c \
	initHome.c \
	pastevents.c \
	peek.c \
	proclore.c \
	prompt.c \
	warp.c \

run:
	@gcc -o whysh -g *.c && ./whysh

clean:
	rm whysh pastevents.txt