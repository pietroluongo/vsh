#ifndef _VSH_H
#define _VSH_H
#include "../include/vsh.h"
#include <signal.h>

#define VSH_EXIT_COMMAND  "exit"
#define VSH_DEBUG_COMMAND "debug"
#define VSH_CLEAR_COMMAND "liberamoita"
#define VSH_NUKE_COMMAND  "armageddon"

// clang-format off
#define EVER ;;
// clang-format on

#define VSH_ALLIGATOR_SIZE 9

static const char* vaccinatedGuy[] = {
    "\n",
    "                  _  _\n",
    "        _ _      (0)(0)-._  _.-'^^'^^'^^'^^'^^'--.\n",
    "       (.(.)----'`        ^^'                /^   ^^-._\n",
    "       (    `                 \\             |    _    ^^-._\n",
    "        VvvvvvvVv~~`__,/.._>  /:/:/:/:/:/:/:/\\  (_..,______^^-.\n",
    "         `^^^^^^^^`/  /   /  /`^^^^^^^^^>^^>^`>  >        _`)  )\n",
    "                   (((`   (((`          (((`  (((`        `'--'^\n",
    "   I feel weird...\n"};

// Auxiliary internal functions
static int isExitCommand(char* command) {
    return !strcmp(command, VSH_EXIT_COMMAND);
}
static int isDebugCommand(char* command) {
    return !strcmp(command, VSH_DEBUG_COMMAND);
}

static int isClearCommand(char* command) {
    return !strcmp(command, VSH_CLEAR_COMMAND);
}

static int isNukeCommand(char* command) {
    return !strcmp(command, VSH_NUKE_COMMAND);
}

static void showProcessExitStatus(int status, pid_t childPid);
static int  readCommandFromStdin(char* whereToStore);
static void printPromptHeader();
static void printAlligator();
static int  execForegroundCommand(CommandData* command);
static int  execBackgroundCommands(CommandDataArray* commandList);
static void handleProcessClear();
static void handleProcessNuke();
void        checkForkError(pid_t pid);
void        handleSIGUSR();
void        setupSignalsToBeIgnored(int isShell);

#endif
