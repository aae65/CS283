#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

void dragon(){                                      //dragon function:
    FILE *file = fopen("dragon.txt", "r");          //open file
    char line[256];                                 //set line buff
    while (fgets(line, sizeof(line), file)) {       //while not at EOF
        printf("%s", line);                         //print line
    }
    printf("\n");                                   //print newline to preserve spacing for rest of code
    fclose(file);                                   //close file
}

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
    char *token;                                    //token var
    char *rest = cmd_line;                          //set rest of line to whole line
    clist->num = 0;                                 //set num to 0

    token = strtok_r(rest, PIPE_STRING, &rest);     //set token until pipe is reached

    while (token != NULL){                          //while token is not null
        if (strcmp(token, EXIT_CMD) == 0){          //if token is exit
            break;                                  //then break
        }

        while (*token == SPACE_CHAR){               //while token is space
            token++;                                //increment token
        }
        char *end = token + strlen(token) - 1;      //create end variable
        while (end > token && *end == SPACE_CHAR){  //while end is greater than token and end is not equal to space
            end--;                                  //decrement end
        }
        *(end + 1) = '\0';                          //set end of end to null terminator

        if (clist->num >= CMD_MAX){                 //if clist is at capacity
            return ERR_TOO_MANY_COMMANDS;           //then return too many commands
        }

        command_t *cmd = &clist->commands[clist->num++];        //create struct for commands and args

        memset(cmd->args, 0, ARG_MAX);                          //clear args to stop repeats

        char *exe_token = strtok(token, " ");                   //create command token
        if (exe_token != NULL){                                 //while command token is not null
            if (strcmp(exe_token, "dragon") == 0){              //if token is dragon
                dragon();                                       //then dragon function
            }
            strncpy(cmd->exe, exe_token, EXE_MAX - 1);          //copy token to list
            cmd->exe[EXE_MAX - 1] = '\0';                       //add null terminating character
        }

        char *arg_token = strtok(NULL, " ");                    //create arg token
        while (arg_token != NULL){                              //while arg token is not null
            strncat(cmd->args, arg_token, ARG_MAX - 1);         //add arg to arg string in list
            cmd->args[ARG_MAX - 1] = '\0';                      //add null terminating character
            arg_token = strtok(NULL, " ");                      //get new arg
        }

        token = strtok_r(rest, PIPE_STRING, &rest);             //get new token
    }

    return OK;                                                  //return ok
}