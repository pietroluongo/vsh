#include "../include/utils.h"
#include <string.h>

void utils_rtrim(char* str) { str[strcspn(str, "\n")] = 0; }

int utils_isChildProcess(pid_t pid) { return pid == 0; }

void utils_closePipe(int pipe[2]) {
    close(pipe[0]);
    close(pipe[1]);
}

void utils_closeAllPipes(int pipe[][2], int pipeCount) {
    for (int i = 0; i < pipeCount; i++) {
        utils_closePipe(pipe[i]);
    }
}