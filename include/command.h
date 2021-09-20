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

void              cmd_printCreatedCommandData(CommandData* cData);
CommandDataArray* cmd_buildCommandStructsFromLine(char* line);
void              cmd_freeCommandDataArray(CommandDataArray* commandData);
void              cmd_checkStatus(int execStatus, char* command);
void              cmd_buildCommandForProgramFromString(char*        command,
                                                       CommandData* commandData);

char* cmd_getCommandProgram(CommandData* command) { return command->argv[0]; };

#endif // VSH_COMMAND_H
