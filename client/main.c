#include <stdio.h>
#include <string.h>

#include "../include/banned.h"

#define MAX_COMMAND_SIZE 64
// clang-format off
#define EVER ;;
// clang-format on

int isExitCommand(char* command) { return !strcmp(command, "exit"); }

int isDebugCommand(char* command) { return !strcmp(command, "debug"); }

void rtrim(char* str) { str[strcspn(str, "\n")] = 0; }

void bolsonaro() {
    char* vaccinatedGuy[] = {
        "\n",
        "                  _  _\n",
        "        _ _      (0)(0)-._  _.-'^^'^^'^^'^^'^^'--.\n",
        "       (.(.)----'`        ^^'                /^   ^^-._\n",
        "       (    `                 \\             |    _    ^^-._\n",
        "        VvvvvvvVv~~`__,/.._>  /:/:/:/:/:/:/:/\\  (_..,______^^-.\n",
        "         `^^^^^^^^`/  /   /  /`^^^^^^^^^>^^>^`>  >        _`)  )\n",
        "                   (((`   (((`          (((`  (((`        `'--'^\n",
        "   I feel weird...\n"};
    for (int i = 0; i < 9; i++) {
        printf("%s", vaccinatedGuy[i]);
    }
}

int main() {
    for (EVER) {
        printf("vsh> ");
        char command[MAX_COMMAND_SIZE];
        fgets(command, MAX_COMMAND_SIZE, stdin);
        rtrim(command);
        if (isExitCommand(command)) {
            break;
        }
        if (isDebugCommand(command)) {
            bolsonaro();
        }
        printf("got command %s\n", command);
    }
    return 0;
}
