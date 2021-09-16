#include "../include/banned.h"
#include "../include/vsh.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>



int main() {
    vsh_setupInitialSignals();
    vsh_mainLoop();
    return 0;
}
