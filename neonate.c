#include "headers.h"
#include "tools.h"

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

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
