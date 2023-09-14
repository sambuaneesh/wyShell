#include "headers.h"
#include "tools.h"
#include <sys/select.h>

pid_t lastProcessID = -1;

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

void neonate(Command *cmd) {
    if (cmd->argc != 3 || strcmp(cmd->argv[1], "-n") != 0) {
        fprintf(stderr, "Usage: %s -n [time_arg]\n", cmd->argv[0]);
        return; // Exit with no effect if the command is not properly formatted
    }

    int time_arg = atoi(cmd->argv[2]);

    char c;
    enableRawMode();
    while (1) {
        // Use the `ps` command to get the latest running process ID
        FILE *ps = popen("ps -e -o pid --sort=-start_time | awk 'NR==2{print $1}'", "r");
        if (ps == NULL) {
            perror("Error executing ps command");
            return;
        }

        char latestProcessIDStr[20];
        if (fgets(latestProcessIDStr, sizeof(latestProcessIDStr), ps) != NULL) {
            lastProcessID = atoi(latestProcessIDStr);
        }
        pclose(ps);

        printf("%d\n", lastProcessID);
        fflush(stdout);

        // Check for input without blocking
        struct timeval tv;
        fd_set fds;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);

        int result = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
        if (result == -1) {
            perror("select");
        } else if (result > 0 && FD_ISSET(STDIN_FILENO, &fds)) {
            // Input is available, read it
            if (read(STDIN_FILENO, &c, 1) == 1) {
                if (c == 'x') {
                    break; // Exit when 'x' is pressed
                }
            }
        }

        // Sleep for 'time_arg' seconds
        sleep(time_arg);
    }

    disableRawMode(); // Restore the terminal to its original mode
}
