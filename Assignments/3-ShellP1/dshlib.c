#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist){
    char *token;
    char *rest = cmd_line;

    clist->num = 0;

    token = strtok_r(rest, PIPE_STRING, &rest);

    if (strcmp(token, "") == 0){
        return WARN_NO_CMDS;
    }

    while (token != NULL){
        if (strcmp(token, EXIT_CMD) == 0){
            break;
        }

        while (*token == SPACE_CHAR){
            token++;
        }
        char *end = token + strlen(token) - 1;
        while (end > token && *end == SPACE_CHAR){
            end--;
        }
        *(end + 1) = '\0';

        if (clist->num >= CMD_MAX){
            return ERR_TOO_MANY_COMMANDS;
        }

        command_t *cmd = &clist->commands[clist->num++];

        char *exe_token = strtok(token, " ");
        if (exe_token != NULL){
            strncpy(cmd->exe, exe_token, EXE_MAX - 1);
            cmd->exe[EXE_MAX - 1] = '\0';
        }

        char *arg_token = strtok(NULL, " ");
        while (arg_token != NULL){
            strncpy(cmd->args, arg_token, ARG_MAX - 1);
            cmd->args[ARG_MAX - 1] = '\0';
            arg_token = strtok(NULL, " ");
        }

        token = strtok_r(rest, PIPE_STRING, &rest);
    }

    return OK;
}