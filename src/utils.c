#include "../include/utils.h"
#include <string.h>

void utils_rtrim(char* str) { str[strcspn(str, "\n")] = 0; }

int utils_isChildProcess(pid_t pid) { return pid == 0; }
