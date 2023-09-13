/*
Command : neonate -n [time_arg]
The command prints the Process-ID of the most recently created process on the system (you are not allowed to use system programs), this pid will be printed every [time_arg] seconds until the key ‘x’ is pressed.
By default , I/O of the terminal is line-buffered, i.e, input is guaranteed to be flushed/sent to your program once a line is terminated.
termios.h, a POSIX-standard header file, allows you to get the tty into raw mode whereas it is generally in cooked mode. Reading it’s documentation/man pages is suggested.
Getting the terminal into raw mode allows it to be such that at soon as a key is pressed, the input signal is sent to your program, along with a lot of other default features like echoing being disabled.
*/

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>

volatile sig_atomic_t shouldExit = 0;

void handleSignal(int signo) {
    if (signo == SIGINT || signo == SIGQUIT) {
        shouldExit = 1;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "-n") != 0) {
        fprintf(stderr, "Usage: %s -n [time_arg]\n", argv[0]);
        return 1;
    }

    int time_arg = atoi(argv[2]);

    struct sigaction sa;
    sa.sa_handler = handleSignal;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    struct termios originalTermios, rawTermios;

    // Get the original terminal attributes
    tcgetattr(STDIN_FILENO, &originalTermios);

    // Enable raw mode for terminal input
    rawTermios = originalTermios;
    rawTermios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawTermios);

    // Set stdin to non-blocking mode
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    // Print the initial PID
    printf("%d\n", getpid());

    while (!shouldExit) {
        // Sleep for the specified time interval
        sleep(time_arg);

        // Check for 'x' keypress
        char c;
        if (read(STDIN_FILENO, &c, 1) == 1 && c == 'x') {
            shouldExit = 1;
        }

        // Get the most recently created process ID
        pid_t pid = getpid();

        // Print the PID
        printf("%d\n", pid);
        fflush(stdout);
    }

    // Restore terminal attributes before exit
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios);

    return 0;
}
