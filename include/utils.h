#ifndef VSH_UTILS_H
#define VSH_UTILS_H
#include <unistd.h>

void utils_rtrim(char* str);
int  utils_isChildProcess(pid_t pid);
void utils_closePipe(int pipe[2]);
void utils_closeAllPipes(int pipe[][2], int pipeCount);

#endif // VSH_UTILS_H
