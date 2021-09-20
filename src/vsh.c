#include "../include/vsh.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/command.h"
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

void closeAllUnusedPipes(int (*fd)[2], int nPipes) {
    for (int i = 0; i < nPipes; i++) {
        close(fd[i][0]);
        close(fd[i][1]);
    }
}

int execBackgroundCommands(CommandDataArray* commandList) {
    int   nCommands = (int)commandList->size;
    pid_t pid[nCommands];
    int   execStat[nCommands];
    int   nPipes = nCommands - 1;
    int   fd[nPipes][2];

    for (int i = 0; i < nPipes; i++) {
        if (pipe(fd[i]) < 0) {
            printf("ERROR creating pipe with index %d\n", i);
            for (int j = 0; j < i; j++) {
                close(fd[j][0]);
                close(fd[j][1]);
            }
            exit(1);
        }
    }

    for (int i = 0; i < nCommands; i++) {
        CommandData* command = &commandList->data[i];
        pid[i] = fork();
        if (pid[i] == -1) {
            printf("ERROR forking with index %d\n", i);
            exit(1);
        }
        int wstatus;
        if (utils_isChildProcess(pid[i])) {
            if (i == 0) {
                // first process needs to stdout to the first pipe
                dup2(fd[i][1], STDOUT_FILENO);
                closeAllUnusedPipes(fd, nPipes);
                execStat[i] = execvp(getCommandProgram(command), command->argv);
                if (execStat < 0) {
                    printf("Erro executando comando %s\n",
                           getCommandProgram(command));
                    exit(1);
                }
            } else if (i == nCommands - 1) {
                // end process needs to stdin from previous pipe
                dup2(fd[i - 1][0], STDIN_FILENO);
                closeAllUnusedPipes(fd, nPipes);
                execStat[i] = execvp(getCommandProgram(command), command->argv);
                if (execStat < 0) {
                    printf("Erro executando comando %s\n",
                           getCommandProgram(command));
                    exit(1);
                }
            } else {
                /* middle processes need to stdin from previous pipe
                 * and stdout to current pipe */
                dup2(fd[i - 1][0], STDIN_FILENO);
                dup2(fd[i][1], STDOUT_FILENO);
                closeAllUnusedPipes(fd, nPipes);
                execStat[i] = execvp(getCommandProgram(command), command->argv);
                if (execStat < 0) {
                    printf("Erro executando comando %s\n",
                           getCommandProgram(command));
                    exit(1);
                }
            }
            freeCommandDataArray(commandList);
            exit(0);
        } else {
            if (i > 0) {
                if (close(fd[i - 1][0])) {
                    printf("error closing pipe %d %d\n", i - 1, 0);
                    exit(1);
                }
                if (close(fd[i - 1][1])) {
                    printf("error closing pipe %d %d\n", i - 1, 0);
                    exit(1);
                }
            }
            waitpid(pid[i], &wstatus, 0);
            showProcessExitStatus(wstatus, pid[i]);
        }
    }
    printf("Finished executing background commands\n");
}

void setBlockedSignals(sigset_t* mask) {
    sigaddset(mask, SIGINT);
    sigaddset(mask, SIGSTOP);
    sigaddset(mask, SIGQUIT);
    int result = sigprocmask(SIG_SETMASK, mask, NULL);
    if (result == -1) {
        printf("Erro ao configurar mascara de processos.\n");
        exit(1);
    }
}

void vsh_setupInitialSignals() {
    sigset_t blockedSignals;
    setBlockedSignals(&blockedSignals);
}
