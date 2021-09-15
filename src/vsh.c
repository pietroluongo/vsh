#include "../include/vsh.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/utils.h"

// clang-format off
#define EVER ;;
// clang-format on

typedef struct commandDataArray {
    CommandData* data;
    ssize_t size;
}CommandDataArray;

// Auxiliary internal functions
int isExitCommand(char* command);
int isDebugCommand(char* command);
void showProcessExitStatus(int status, pid_t childPid);
void printCreatedCommandData(CommandData* cData);
void readCommandFromStdin(char* whereToStore);
char* getCommandProgram(CommandData* command);
CommandDataArray* buildCommandStructsFromLine(char* line);
void freeCommandDataArray(CommandDataArray* commandData);
void printPromptHeader();
void bolsonaro();

void bolsonaro() {
    char* vaccinatedGuy[] = {
        "\n",
        "                  _  _\n",
        "        _ _      (0)(0)-._  _.-'^^'^^'^^'^^'^^'--.\n",
        "       (.(.)----'`        ^^'                /^   ^^-._\n",
        "       (    `                 \\             |    _    ^^-._\n",
        "        VvvvvvvVv~~`__,/.._>  /:/:/:/:/:/:/:/\\  (_..,______^^-.\n",
        "         `^^^^^^^^`/  /   /  /`^^^^^^^^^>^^>^`>  >        _`)  )\n",
        "                   (((`   (((`          (((`  (((`        `'--'^\n",
        "   I feel weird...\n"};
    for (int i = 0; i < 9; i++) {
        printf("%s", vaccinatedGuy[i]);
    }
}

int isExitCommand(char* command) { return !strcmp(command, "exit"); }

int isDebugCommand(char* command) { return !strcmp(command, "debug"); }

void showProcessExitStatus(int wstatus, pid_t childPid) {
    if (WIFEXITED(wstatus)) {
        printf("[Shell] Process %ld exited with code %d\n", childPid,
               WEXITSTATUS(wstatus));
    }
    if (WIFSIGNALED(wstatus)) {
        printf("[Shell] Process %ld signaled with code %ld\n", childPid,
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

CommandDataArray* buildCommandStructsFromLine(char* line) {
    CommandDataArray* arr = malloc(sizeof(CommandDataArray));
    CommandData* commands = malloc(MAX_COMMANDS_PER_LINE * sizeof(CommandData));
    int iterator = 0;
    char* separatedCommands = strtok(line, "|");
    while (separatedCommands) {
        printf("Separated command: %s\n", separatedCommands);
        char* separatedArgs = strtok(line, " ");
        char iterator2 = 0;
        while (separatedArgs) {
        }
        separatedCommands = strtok(NULL, "|");
    }
    arr->size = 0;
    arr->data = commands;
    return arr;
}

void readCommandFromStdin(char* whereToStore) {
    fgets(whereToStore, MAX_COMMAND_SIZE, stdin);
    utils_rtrim(whereToStore);
}

void printPromptHeader() { printf("vsh> "); }

// Linha: Array<CommandData>
// ls | grep "pastel"
// [{command: "ls", argv: ["ls"], argc: 1}, {command: "grep", }]

void vsh_mainLoop() {
    for (EVER) {
        printPromptHeader();
        char command[MAX_COMMAND_SIZE];
        readCommandFromStdin(command);
        CommandDataArray* parsedCommandList = buildCommandStructsFromLine(command);
        CommandData* parsedCommands = parsedCommandList->data;
        if (isExitCommand(command)) {
            break;
        }
        if (isDebugCommand(command)) {
            bolsonaro();
        }
        char** argv = malloc(1 * sizeof(char*));
        argv[0] = command;
        CommandData commandData;
        commandData.argv = argv;
        commandData.argc = 1;
        printCreatedCommandData(&commandData);
        execForegroundCommand(&commandData);
        free(argv);
        freeCommandDataArray(parsedCommandList);
    }
}

void freeCommandDataArray(CommandDataArray * commandData) {
    //    for(int i = 0; i < commandData->size; i++) {
    //        free(commandData->data[i]);
    //    }
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
        char** param = command->argv;
        int execStat = execvp(getCommandProgram(command), command->argv);
        if (execStat < 0) {
            printf("Erro executando comando %s\n", command);
            exit(1);
        }
    } else {
        waitpid(pid, &wstatus, 0);
        showProcessExitStatus(wstatus, pid);
    }
}