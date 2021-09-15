#ifndef VSH_H
#define VSH_H

#define MAX_COMMAND_SIZE 64

typedef struct commandData {
    char* command;
    char** argv;
    int argc;
}CommandData;

void vsh_mainLoop();
int execForegroundCommand(CommandData* command);

#endif
