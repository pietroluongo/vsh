#include "../include/vsh.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/command.h"
#include "../include/utils.h"
#include "./_vsh.h"

#define USR1_SIGNAL 10
#define USR2_SIGNAL 12
#define READ        0
#define WRITE       1

#define MAX_PGREP_LENGTH 1024

void printAlligator() {
    for (int i = 0; i < VSH_ALLIGATOR_SIZE; i++) {
        printf("%s", vaccinatedGuy[i]);
    }
}

void checkForkError(pid_t pid) {
    if (pid == -1) {
        printf("Error: fork failed!\n");
        exit(1);
    }
}

void handleSIGUSR() {
    printAlligator();
    printPromptHeader();
    fflush(stdout);
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

int readCommandFromStdin(char* whereToStore) {
    if (fgets(whereToStore, MAX_COMMAND_SIZE, stdin)) {
        utils_rtrim(whereToStore);
        return 1;
    }
    return 0;
}

void printPromptHeader() {
    printf("\033[0;34m");
    printf("vsh> ");
    printf("\033[0m");
}

void handleProcessClear() {
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0)
        ;
}

void handleProcessNuke() {
    FILE* pgrepResult;
    char  pgrepCmd[MAX_PGREP_LENGTH];
    snprintf(pgrepCmd, MAX_PGREP_LENGTH, "/usr/bin/pgrep -P %d -r RS", getpid());
    pgrepResult = popen(pgrepCmd, "r");

    if (pgrepResult == NULL) {
        printf("ERROR opening popen\n");
        exit(1);
    }

    while (fgets(pgrepCmd, sizeof(pgrepCmd), pgrepResult) != NULL) {
        int pid = atoi(pgrepCmd);
        printf("killing process %d and parent is %d\n", pid, getppid());
        killpg(pid, SIGKILL);
    }
    fclose(pgrepResult);
}

void vsh_mainLoop() {
    setupForegroundSignalsToBeIgnored(1);
    for (EVER) {
        printPromptHeader();
        char command[MAX_COMMAND_SIZE];
        if (!readCommandFromStdin(command)) {
            break;
        }
        if (isExitCommand(command)) {
            break;
        } else if (isDebugCommand(command)) {
            printAlligator();
            continue;
        } else if (isClearCommand(command)) {
            handleProcessClear();
            continue;
        } else if (isNukeCommand(command)) {
            handleProcessNuke();
            continue;
        }
        CommandDataArray* parsedCommandList =
            cmd_buildCommandStructsFromLine(command);
        CommandData* parsedCommands = parsedCommandList->data;
        if (parsedCommandList->size == 0) {
            continue;
        } else if (parsedCommandList->size == 1) {
            execForegroundCommand(&parsedCommands[0]);
        } else {
            execBackgroundCommands(parsedCommandList);
        }
        cmd_freeCommandDataArray(parsedCommandList);
    }
}

int execForegroundCommand(CommandData* command) {
    pid_t pid;
    pid = fork();
    checkForkError(pid);
    int wstatus;
    if (utils_isChildProcess(pid)) {
        setupForegroundSignalsToBeIgnored(0);
        int execStatus = execvp(getCommandProgram(command), command->argv);
        cmd_checkStatus(execStatus, getCommandProgram(command));
    } else {
        waitpid(pid, &wstatus, 0);
        showProcessExitStatus(wstatus, pid);
    }
}

void handleSIGUSRInBackground() {
    pid_t group_id = getpgid(getpid());
    killpg(group_id, SIGKILL);
}

int execBackgroundCommands(CommandDataArray* commandList) {
    pid_t proxyPID = fork();
    checkForkError(proxyPID);
    if (utils_isChildProcess(proxyPID)) {
        setsid();
        int   nCommands = (int)commandList->size;
        pid_t pid[nCommands];
        int   execStatus[nCommands];
        int   nPipes = nCommands - 1;
        int   pipeDescriptors[nPipes][2];

        for (int i = 0; i < nPipes; i++) {
            if (pipe(pipeDescriptors[i]) < 0) {
                printf("ERROR creating pipe with index %d\n", i);
                utils_closeAllPipes(pipeDescriptors, i);
                exit(1);
            }
        }

        for (int i = 0; i < nCommands; i++) {
            CommandData* command = &commandList->data[i];
            pid[i] = fork();
            checkForkError(pid[i]);
            int wstatus = 0;
            if (utils_isChildProcess(pid[i])) {
                printf("PID %d setting up signals\n", (int)getpid());
                setupBackgroundSignalsToBeIgnored();
                if (i == 0) {
                    // first process needs to stdout to the first pipe
                    dup2(pipeDescriptors[i][WRITE], STDOUT_FILENO);
                } else if (i == nCommands - 1) {
                    // end process needs to stdin from previous pipe
                    dup2(pipeDescriptors[i - 1][READ], STDIN_FILENO);
                } else {
                    /* middle processes need to stdin from previous pipe
                     * and stdout to current pipe */
                    dup2(pipeDescriptors[i - 1][READ], STDIN_FILENO);
                    dup2(pipeDescriptors[i][WRITE], STDOUT_FILENO);
                }
                utils_closeAllPipes(pipeDescriptors, nPipes);
                execStatus[i] =
                    execvp(getCommandProgram(command), command->argv);
                cmd_checkStatus(execStatus[i], getCommandProgram(command));
                cmd_freeCommandDataArray(commandList);
                exit(0);
            } else {
                if (i > 0) {
                    if (close(pipeDescriptors[i - 1][READ])) {
                        printf("error closing pipe %d %d\n", i - 1, 0);
                        exit(1);
                    }
                    if (close(pipeDescriptors[i - 1][WRITE])) {
                        printf("error closing pipe %d %d\n", i - 1, 0);
                        exit(1);
                    }
                }
                showProcessExitStatus(wstatus, pid[i]);
            }
        }
        int status;
        while (1) {
            if (((waitpid(-1, &status, WNOHANG)) == -1) && (errno == ECHILD))
                break;

            if (status == USR1_SIGNAL || status == USR2_SIGNAL) {
                handleSIGUSRInBackground();
            }
        }
        exit(0);
    }
    printf("Finished executing background commands\n");
}

void setupForegroundSignalsToBeIgnored(int isShell) {
    sigset_t         blockedSignals;
    struct sigaction handler;
    sigemptyset(&blockedSignals);
    if (isShell) {
        sigaddset(&blockedSignals, SIGINT);
        sigaddset(&blockedSignals, SIGTSTP);
        sigaddset(&blockedSignals, SIGQUIT);
        handler.sa_handler = &handleSIGUSR;
        if (sigprocmask(SIG_SETMASK, &blockedSignals, NULL) == -1) {
            printf("Erro ao configurar mascara de processos.\n");
            exit(1);
        }
    } else {
        handler.sa_handler = SIG_IGN;
    }
    handler.sa_mask = blockedSignals;
    handler.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &handler, NULL);
    sigaction(SIGUSR2, &handler, NULL);
}

void setupBackgroundSignalsToBeIgnored() {
    struct sigaction handler;
    handler.sa_handler = SIG_IGN;
    sigemptyset(&(handler.sa_mask));
    handler.sa_flags = SA_RESTART;
    sigaction(SIGINT, &handler, NULL);
    sigaction(SIGTSTP, &handler, NULL);
    sigaction(SIGQUIT, &handler, NULL);
    printf("PROCESS %d IGNOREING\n", (int)getpid());
}