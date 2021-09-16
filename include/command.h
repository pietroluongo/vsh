#ifndef VSH_COMMAND_H
#define VSH_COMMAND_H
#include <stdlib.h>

#define MAX_COMMAND_SIZE      1024
#define MAX_COMMAND_ARGS      3
#define MAX_COMMANDS_PER_LINE 5

typedef struct commandData {
    char** argv;
    int    argc;
} CommandData;

typedef struct commandDataArray {
    CommandData* data;
    ssize_t      size;
} CommandDataArray;

void              printCreatedCommandData(CommandData* cData);
CommandDataArray* buildCommandStructsFromLine(char* line);
char*             getCommandProgram(CommandData* command);
void              freeCommandDataArray(CommandDataArray* commandData);
void              buildCommandForProgramFromString(char*        command,
                                                          CommandData* commandData);

#endif // VSH_COMMAND_H
