#include "../include/vsh.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/utils.h"
#include "./_vsh.h"

void printAlligator() {
    for (int i = 0; i < VSH_ALLIGATOR_SIZE; i++) {
        printf("%s", vaccinatedGuy[i]);
    }
}

void showProcessExitStatus(int wstatus, pid_t childPid) {
    if (WIFEXITED(wstatus)) {
        printf("[Shell] Process %d exited with code %d\n", (int)childPid,
               WEXITSTATUS(wstatus));
    }
    if (WIFSIGNALED(wstatus)) {
        printf("[Shell] Process %d signaled with code %d\n", (int)childPid,
               WTERMSIG(wstatus));
    }
}

char* getCommandProgram(CommandData* command) { return command->argv[0]; }

void printCreatedCommandData(CommandData* cData) {
    printf("---------------[ Command ]---------------\n");
    printf("program: %s\n", getCommandProgram(cData));
    printf("args: [ ");
    for (int i = 0; i < cData->argc - 1; i++) {
        printf("%s, ", cData->argv[i]);
    }
    printf("%s ]\n", cData->argv[cData->argc - 1]);
    printf("-----------------------------------------\n");
}

void buildCommandForProgramFromString(char* command, CommandData* commandData) {
    char** argv = malloc(MAX_COMMAND_ARGS * sizeof(char*));
    char*  separatedArgsEnd;
    char*  separatedArgs = strtok_r(command, " ", &separatedArgsEnd);
    int    it = 0;
    while (separatedArgs) {
        argv[it++] = separatedArgs;
        separatedArgs = strtok_r(NULL, " ", &separatedArgsEnd);
    }
    commandData->argv = argv;
    commandData->argc = it;
    printCreatedCommandData(commandData);
}

CommandDataArray* buildCommandStructsFromLine(char* line) {
    CommandDataArray* arr = malloc(sizeof(CommandDataArray));
    CommandData* commands = malloc(MAX_COMMANDS_PER_LINE * sizeof(CommandData));
    char*        separatedCommandsEnd;
    char*        separatedCommands = strtok_r(line, "|", &separatedCommandsEnd);
    int          it = 0;
    while (separatedCommands) {
        buildCommandForProgramFromString(separatedCommands, &commands[it++]);
        separatedCommands = strtok_r(NULL, "|", &separatedCommandsEnd);
    }
    arr->size = it;
    arr->data = commands;
    return arr;
}

void readCommandFromStdin(char* whereToStore) {
    fgets(whereToStore, MAX_COMMAND_SIZE, stdin);
    utils_rtrim(whereToStore);
}

void printPromptHeader() { printf("vsh> "); }

void handleProcessClear() {
    printf("[Shell] TODO: implement process clearing logic\n");
}

void handleProcessNuke() {
    printf("[Shell] TODO: implement process nuking logic\n");
}

void vsh_mainLoop() {
    for (EVER) {
        printPromptHeader();
        char command[MAX_COMMAND_SIZE];
        readCommandFromStdin(command);
        if (isExitCommand(command)) {
            break;
        } else if (isDebugCommand(command)) {
            printAlligator();
            continue;
        } else if (isClearCommand(command)) {
            handleProcessClear();
        } else if (isNukeCommand(command)) {
            handleProcessNuke();
        }
        CommandDataArray* parsedCommandList =
            buildCommandStructsFromLine(command);
        CommandData* parsedCommands = parsedCommandList->data;
        if (parsedCommandList->size == 0) {
            continue;
        } else if (parsedCommandList->size == 1) {
            execForegroundCommand(&parsedCommands[0]);
        } else {
            execBackgroundCommands(parsedCommandList);
        }
        freeCommandDataArray(parsedCommandList);
    }
}

void freeCommandDataArray(CommandDataArray* commandData) {
    for (int i = 0; i < commandData->size; i++) {
        free(commandData->data[i].argv);
    }
    free(commandData->data);
    free(commandData);
}

int execForegroundCommand(CommandData* command) {
    pid_t pid;
    pid = fork();
    if (pid == -1) {
        printf("Erro executando o fork\n");
        exit(1);
    }
    int wstatus;
    if (utils_isChildProcess(pid)) {
        int execStat = execvp(getCommandProgram(command), command->argv);
        if (execStat < 0) {
            printf("Erro executando comando %s\n", getCommandProgram(command));
            exit(1);
        }
    } else {
        waitpid(pid, &wstatus, 0);
        showProcessExitStatus(wstatus, pid);
    }
}

int execBackgroundCommands(CommandDataArray* commandList) {
    printf("TODO: implement execBackgroundCommand()\n");
}