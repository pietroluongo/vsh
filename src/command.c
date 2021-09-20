#include "../include/command.h"
#include <stdio.h>
#include <string.h>

void cmd_printCreatedCommandData(CommandData* cData) {
    printf("---------------[ Command ]---------------\n");
    printf("program: %s\n", cmd_getCommandProgram(cData));
    printf("args: [ ");
    for (int i = 0; i < cData->argc - 1; i++) {
        printf("%s, ", cData->argv[i]);
    }
    printf("%s ]\n", cData->argv[cData->argc - 1]);
    printf("-----------------------------------------\n");
}

void cmd_buildCommandForProgramFromString(char*        command,
                                          CommandData* commandData) {
    char** argv = malloc((MAX_COMMAND_ARGS * sizeof(char*)) + 1);
    char*  separatedArgsEnd;
    char*  separatedArgs = strtok_r(command, " ", &separatedArgsEnd);
    int    it = 0;
    while (separatedArgs) {
        argv[it++] = separatedArgs;
        separatedArgs = strtok_r(NULL, " ", &separatedArgsEnd);
    }
    commandData->argv = argv;
    commandData->argc = it;
    cmd_printCreatedCommandData(commandData);
}

CommandDataArray* cmd_buildCommandStructsFromLine(char* line) {
    CommandDataArray* arr = malloc(sizeof(CommandDataArray));
    CommandData* commands = malloc(MAX_COMMANDS_PER_LINE * sizeof(CommandData));
    char*        separatedCommandsEnd;
    char*        separatedCommands = strtok_r(line, "|", &separatedCommandsEnd);
    int          it = 0;
    while (separatedCommands) {
        cmd_buildCommandForProgramFromString(separatedCommands,
                                             &commands[it++]);
        separatedCommands = strtok_r(NULL, "|", &separatedCommandsEnd);
    }
    arr->size = it;
    arr->data = commands;
    return arr;
}

void cmd_freeCommandDataArray(CommandDataArray* commandData) {
    for (int i = 0; i < commandData->size; i++) {
        free(commandData->data[i].argv);
    }
    free(commandData->data);
    free(commandData);
}