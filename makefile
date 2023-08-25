main:
	gcc -o whysh -g \
    main.c \
    prompt.c \
    initHome.c \
    command.c \
    handleInput.c \
    warp.c \
    executeCommand.c


clean:
	rm *.out