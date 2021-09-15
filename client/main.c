#include "../include/banned.h"
#include "../include/vsh.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void setBlockedSignals(sigset_t* mask) {
    sigaddset(mask, SIGINT);
    sigaddset(mask, SIGSTOP);
    sigaddset(mask, SIGQUIT);
    int result = sigprocmask(SIG_SETMASK, mask, NULL);
    if(result == -1) {
        printf("Erro ao configurar mascara de processos.\n");
        exit(1);
    }
    return;
}

int main() {
    sigset_t blockedSignals;
    setBlockedSignals(&blockedSignals);
    vsh_mainLoop();
    return 0;
}
