#ifndef VSH_H
#define VSH_H
#include <stdlib.h>

#define MAX_COMMAND_SIZE      4096
#define MAX_COMMAND_ARGS      64
#define MAX_COMMANDS_PER_LINE 5

typedef struct commandData {
    char** argv;
    int argc;
} CommandData;

typedef struct commandDataArray {
    CommandData* data;
    ssize_t size;
} CommandDataArray;

void vsh_mainLoop();
int execForegroundCommand(CommandData* command);
int execBackgroundCommands(CommandDataArray* commandList);

#endif
