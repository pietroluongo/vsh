#include <stdio.h>
#include <string.h>
#define MAX_COMMAND_SIZE 64
// clang-format off
#define EVER ;;
// clang-format on

int isExitCommand(char* command) { return !strcmp(command, "exit"); }

void rtrim(char* str) {
    str[strcspn(str, "\n")] = 0;
}

int main() {
    for (EVER) {
        printf("vsh> ");
        char command[MAX_COMMAND_SIZE];
        fgets(command, MAX_COMMAND_SIZE, stdin);
        rtrim(command);
        if (isExitCommand(command)) { break; }
        printf("got command %s\n", command);
    }
    return 0;
}
