#ifndef _VSH_H
#define _VSH_H
#include "../include/vsh.h"

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

typedef struct commandData {
    char** argv;
    int    argc;
} CommandData;

typedef struct commandDataArray {
    CommandData* data;
    ssize_t      size;
} CommandDataArray;

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

static void              showProcessExitStatus(int status, pid_t childPid);
static void              printCreatedCommandData(CommandData* cData);
static void              readCommandFromStdin(char* whereToStore);
static char*             getCommandProgram(CommandData* command);
static CommandDataArray* buildCommandStructsFromLine(char* line);
static void              freeCommandDataArray(CommandDataArray* commandData);
static void              printPromptHeader();
static void              printAlligator();
static void              buildCommandForProgramFromString(char*        command,
                                                          CommandData* commandData);
static int               execForegroundCommand(CommandData* command);
static int               execBackgroundCommands(CommandDataArray* commandList);
static void              handleProcessClear();
static void              handleProcessNuke();

#endif
