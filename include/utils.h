#ifndef VSH_UTILS_H
#define VSH_UTILS_H
#include <unistd.h>

void utils_rtrim(char* str);
int  utils_isChildProcess(pid_t pid);

#endif // VSH_UTILS_H
