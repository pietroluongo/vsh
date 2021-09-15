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

// Auxiliary internal functions
int isExitCommand(char* command);
int isDebugCommand(char* command);
void showProcessExitStatus(int status, pid_t childPid);
void printCreatedCommandData(CommandData* cData);
void readCommandFromStdin(char* whereToStore);
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

void printCreatedCommandData(CommandData* cData) {
    printf("---------------[ Command ]---------------\n");
    printf("command: %s\n", cData->command);
    printf("args: [");
    for(int i = 0; i < cData->argc-1; i++) {
        printf("%s, ", cData->argv[i]);
    }
    printf("%s ]\n", cData->argv[cData->argc-1]);
    printf("-----------------------------------------\n");
}

void readCommandFromStdin(char* whereToStore) {
    fgets(whereToStore, MAX_COMMAND_SIZE, stdin);
    utils_rtrim(whereToStore);
}

void printPromptHeader() {
    printf("vsh> ");
}

void vsh_mainLoop() {
    for (EVER) {
        printPromptHeader();
        char command[MAX_COMMAND_SIZE];
        readCommandFromStdin(command);
        if (isExitCommand(command)) {
            break;
        }
        if (isDebugCommand(command)) {
            bolsonaro();
        }
        CommandData commandData;
        commandData.command = command;
        char* a[1];
        a[0] = "";
        commandData.argv = a;
        commandData.argc = 1;
        printCreatedCommandData(&commandData);
        execForegroundCommand(&commandData);
    }
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
        int execStat = execvp(command->command, &command->command);
        if (execStat < 0) {
            printf("Erro executando comando %s\n", command);
            exit(1);
        }
    } else {
        waitpid(pid, &wstatus, 0);
        showProcessExitStatus(wstatus, pid);
    }
}