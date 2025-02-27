#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

//build cmd_buff function
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff){
    if (cmd_line == NULL || cmd_buff == NULL){              //if a line could not be filled
        return ERR_MEMORY;                                  //return memory error
    }

    free_cmd_buff(cmd_buff);                               //clear buff to prevent errors

    cmd_buff->_cmd_buffer = strdup(cmd_line);               //duplicate line into buffer for struct
    if (cmd_buff->_cmd_buffer == NULL){                     //if buffer is empty
        return ERR_MEMORY;                                  //then return memory error
    }

    char *token = cmd_buff->_cmd_buffer;                            //create token and set to buffer
    while (*token != '\0' && cmd_buff->argc < CMD_ARGV_MAX - 1){    //while token is not ended and argc is less than max
        while (isspace(*token)){                                    //while token is space
            token++;                                                //continue through token
        }

        char *stripped_token = token;                               //create stripped token variable
        if (*token == '"' || *token == '\''){                       //if part of token is a quotation
            char quote = *token++;                                  //then create variable and continue through token
            stripped_token = token;                                 //set stripped token to current token
            while (*token != '\0' && *token != quote) {             //while token is not ended and token is not quote
                token++;                                            //continue through token
            }
            if (*token == quote){                                   //if token is quote
                *token++ = '\0';                                    //set end of token to null terminating character
            }
        } else {
            while (*token != '\0' && !isspace(*token)){             //while token is not ended and token is not space
                token++;                                            //move through token
            }
        }

        if (*token != '\0') {                                       //if token is not ended
            *token++ = '\0';                                        //then set end of token to null terminating character
        }
        
        if (cmd_buff->argc >= CMD_ARGV_MAX - 1) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
        
        cmd_buff->argv[cmd_buff->argc++] = stripped_token;          //set next argument of struct to stripped token
    }
    cmd_buff->argv[cmd_buff->argc] = NULL;                          //set last argument of struct to null

    if (cmd_buff->argc >= CMD_ARGV_MAX - 1){                        //if argc is greater than max
        return ERR_CMD_OR_ARGS_TOO_BIG;                             //then return error
    }

    return OK;
}

int free_cmd_list(command_list_t *cmd_list){
    free(cmd_list);

    return OK;
}

//clear cmd buff function
int clear_cmd_buff(cmd_buff_t *cmd_buff){
    for (int i = 0; i < CMD_ARGV_MAX; i++){         //loop through all arguments
        cmd_buff->argv[i] = NULL;                   //set each to null
    }
    cmd_buff->argc = 0;                             //reset argc counter

    return OK;
}

//free cmd buff function
int free_cmd_buff(cmd_buff_t *cmd_buff){
    if (cmd_buff == NULL){                          //if cmd_buff is empty
        return ERR_MEMORY;                          //then return memory error
    }

    clear_cmd_buff(cmd_buff);

    if (cmd_buff->_cmd_buffer != NULL){             //if buffer is not empty
        free(cmd_buff->_cmd_buffer);                //then free buffer
        cmd_buff->_cmd_buffer = NULL;               //and set to null
    }

    return OK;
}

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

int exec_local_cmd_loop()
{
    char cmd_buff[ARG_MAX];
    int rc = 0;
    cmd_buff_t *cmd = calloc(1, sizeof(cmd_buff_t));
    command_list_t *clist = calloc(1, sizeof(command_list_t));

    while (1){
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
            printf("\n");
            break;
        }
        //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';

        if (strcmp(cmd_buff, EXIT_CMD) == 0){
            rc = EXIT_SUCCESS;
            break;
        }

        if (strcmp(cmd_buff, "") == 0){
            printf(CMD_WARN_NO_CMD);
            continue;
        }
        
        rc = build_cmd_list(cmd_buff, clist);                       //set return code to build_cmd_list function
        if (rc == ERR_MEMORY){                                      //if there is a memory error
            break;                                                  //then break
        } else if (rc == ERR_CMD_OR_ARGS_TOO_BIG || rc == ERR_TOO_MANY_COMMANDS){                  //if there is an argument error
            printf(CMD_ERR_PIPE_LIMIT, 8);
            break;                                                  //then break
        } else {
            for (int i = 0; i < clist->num; i++){
                //printf("%s: %s\n", clist->commands[i].exe, clist->commands[i].args);
                command_t *curr_cmd = &clist->commands[i];

                rc = build_cmd_buff(curr_cmd->args, cmd);
                if (rc != OK){
                    fprintf(stderr, "Error building buffer\n");
                    break;
                }

                if (strcmp(curr_cmd->exe, "dragon") == 0){               //if command is dragon
                    print_dragon();                                     //then print_dragon command
                } else if (strcmp(curr_cmd->exe, "cd") == 0){            //else if command is cd
                    if (curr_cmd->args != NULL) {                         //then if the next argument is not null
                        chdir(curr_cmd->args);                            //change directory to next argument
                    }
                } else {                    
                    pid_t pid = fork();                                 //create pid_t variable and fork
                    if (pid == 0){                                      //if fork was ok
                        execvp(curr_cmd->exe, cmd->argv);               //then execvp
                        rc = OK;                                        //set rc to exit fail
                        break;                                          //and break
                    } else if (pid > 0){                                //else if fork called for status
                        int status;                                     //then create status var
                        waitpid(pid, &status, 0);                       //wait for status
                    } else {    
                        rc = ERR_EXEC_CMD;                              //set rc to error
                        break;                                          //break
                    }
                }
            }
        }
    
    }
    free_cmd_buff(cmd);                 //free cmd_buff
    free_cmd_list(clist);              //free cmd

    return rc;              //return rc
}
