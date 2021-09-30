#ifndef VSH_COMMAND_H
#define VSH_COMMAND_H
#include <stdlib.h>

#define MAX_COMMAND_SIZE      1024
#define MAX_COMMAND_ARGS      3
#define MAX_COMMANDS_PER_LINE 5

typedef struct commandData {
    char** argv;
    int    argc;
} CommandData;

typedef struct commandDataArray {
    CommandData* data;
    ssize_t      size;
} CommandDataArray;

/**
 * Debug command to show a given command's stored data
 * @param cData The command to be debugged
 */
void cmd_printCreatedCommandData(CommandData* cData);
/**
 * Builds a command struct array from a single terminal line
 * @param line The line from which the commands should be built
 * @return The array of built commands
 */
CommandDataArray* cmd_buildCommandStructsFromLine(char* line);
/**
 * Frees the memory from a given command data array
 * @param commandData The array that should be freed
 */
void cmd_freeCommandDataArray(CommandDataArray* commandData);
/**
 * Checks if the execution of a given command was successful
 * @param execStatus The return from the command's exec() call
 * @param command the command itself
 */
void cmd_checkStatus(int execStatus, char* command);
/**
 * Builds a command for a program from a given string
 * @param command The string that contains the actual command
 * @param commandData The memory space where the command data shall be stored
 */
void cmd_buildCommandForProgramFromString(char*        command,
                                          CommandData* commandData);
/**
 * Gets the command from a given CommandData struct
 * @param command The command struct from which to retrieve the command
 * @return A string containing the command
 */
char* cmd_getCommandProgram(CommandData* command);
#endif // VSH_COMMAND_H
