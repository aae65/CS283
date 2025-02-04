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
    char *arg_token;

    clist->num = 0;

    token = strtok(cmd_line, PIPE_STRING);
    while (token != NULL){
        while (*token == SPACE_CHAR){
            token++;
        }
        char *end = token + strlen(token) - 1;
        while (end > token && *end == SPACE_CHAR){
            end--;
        }
        *(end + 1) = '\0';

        if (strlen(token) >= ARG_MAX){
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        if (clist->num < CMD_MAX){
            arg_token = strtok(token, SPACE_CHAR);
            if (arg_token != NULL){
                strncpy(clist->commands[clist->num].exe, arg_token, EXE_MAX - 1);
                clist->commands[clist->num].exe[EXE_MAX - 1] = '\0';

                char *args = token + strlen(arg_token) + 1;
                strncpy(clist->commands[clist->num].args, args, ARG_MAX - 1);
                clist->commands[clist->num].args[ARG_MAX - 1] = '\0';
            }
            clist->num++;
        } else {
            return ERR_TOO_MANY_COMMANDS;
        }

        token = strtok(NULL, PIPE_STRING);
    }

    return OK;
}