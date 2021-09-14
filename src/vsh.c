#include "../include/vsh.h"

#include "../include/utils.h"
#include <stdio.h>
#include <string.h>

// clang-format off
#define EVER ;;
// clang-format on


// Auxiliary internal functions
int isExitCommand(char* command);
int isDebugCommand(char* command);
void bolsonaro();


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

int isExitCommand(char* command) { return !strcmp(command, "exit"); }

int isDebugCommand(char* command) { return !strcmp(command, "debug"); }

void vsh_mainLoop() {
    for (EVER) {
        printf("vsh> ");
        char command[MAX_COMMAND_SIZE];
        fgets(command, MAX_COMMAND_SIZE, stdin);
        utils_rtrim(command);
        if (isExitCommand(command)) {
            break;
        }
        if (isDebugCommand(command)) {
            bolsonaro();
        }
        printf("got command %s\n", command);
    }
}