#ifndef VSH_UTILS_H
#define VSH_UTILS_H
#include <unistd.h>

/**
 * Trims spaces from the right of a given string.
 * @param str The string to be trimmed
 */
void utils_rtrim(char* str);
/**
 * Checks if a given PID belongs to a child process post-fork
 * @param pid PID to be checked
 * @return Bool-like that simbolizes whether the passed pid is a child process
 */
int utils_isChildProcess(pid_t pid);
/**
 * Closes a given pipe
 * @param pipe the pipe to be closed
 */
void utils_closePipe(int pipe[2]);

/**
 * Closes an array of pipes
 * @param pipe the pipe array to be closed
 * @param pipeCount how many pipes there are on the pipe array
 */
void utils_closeAllPipes(int pipe[][2], int pipeCount);

#endif // VSH_UTILS_H
