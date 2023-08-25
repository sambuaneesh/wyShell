#include "headers.h"
#include "prompt.h"
#include "tools.h"

char *global_home = NULL;

int main()
{
    global_home = initializeGlobalHome();
    while (1)
    {
        prompt();
        char input[4096];
        fgets(input, 4096, stdin);
    }
}
