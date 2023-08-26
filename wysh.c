#include "headers.h"

void printWyshArt()
{
    printf(BOLD_CYAN_COLOR
           "                                     ,---,       \n"
           "                                   ,--.' |       \n"
           "         .---.                     |  |  :       \n"
           "        /. ./|            .--.--.  :  :  :       \n"
           "     .-'-. ' |      .--, /  /    ' :  |  |,--.   \n"
           "    /___/ \\: |    /_ ./||  :  /`./ |  :  '   |  \n"
           " .-'.. '   ' . , ' , ' :|  :  ;_   |  |   /' :  \n"
           "/___/ \\:     '/___/ \\: | \\  \\    `.'  :  | | |  \n"
           ".   \\  ' .\\    .  \\  ' |  `----.   \\  |  ' | :  \n"
           " \\   \\" BOLD_YELLOW_COLOR "  ' .\\ |  \\  ;   : /  /`--'  /  :  :_:,' \n" RESET_COLOR
           "  \\   \\  |--\"" BOLD_MAGENTA_COLOR "    \\  \\  ;'--'.     /|  | ,'     \n" RESET_COLOR
           "   \\   \\" BOLD_MAGENTA_COLOR " |" RESET_COLOR "        :  \\  \\" BOLD_CYAN_COLOR "  ;`--'---' `--''       \n" RESET_COLOR
           "    '---\"          \\  ' ;                      \n"
           "                    `--`                        \n");

    printf(BOLD_YELLOW_COLOR "Welcome to wyShe!!\n" RESET_COLOR);
}
