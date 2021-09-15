#ifndef VSH_H
#define VSH_H

#define MAX_COMMAND_SIZE        4096
#define MAX_COMMAND_ARGS        64
#define MAX_COMMANDS_PER_LINE   5

typedef struct commandData {
    char** argv;
    int argc;
} CommandData;

void vsh_mainLoop();
int execForegroundCommand(CommandData* command);

#endif
