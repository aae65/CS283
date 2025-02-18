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

        cmd_buff->argv[cmd_buff->argc++] = stripped_token;          //set next argument of struct to stripped token
    }
    cmd_buff->argv[cmd_buff->argc] = NULL;                          //set last argument of struct to null

    if (cmd_buff->argc >= CMD_ARGV_MAX - 1){                        //if argc is greater than max
        return ERR_CMD_OR_ARGS_TOO_BIG;                             //then return error
    }

    return OK;
}

//free cmd buff function
int free_cmd_buff(cmd_buff_t *cmd_buff){
    if (cmd_buff == NULL){                          //if cmd_buff is empty
        return ERR_MEMORY;                          //then return memory error
    }

    if (cmd_buff->_cmd_buffer != NULL){             //if buffer is not empty
        free(cmd_buff->_cmd_buffer);                //then free buffer
        cmd_buff->_cmd_buffer = NULL;               //and set to null
    }

    for (int i = 0; i < CMD_ARGV_MAX; i++){         //loop through all arguments
        cmd_buff->argv[i] = NULL;                   //set each to null
    }
    cmd_buff->argc = 0;                             //reset argc counter

    return OK;
}

int exec_local_cmd_loop()
{
    char cmd_buff[ARG_MAX];
    int rc = 0;
    cmd_buff_t *cmd = malloc(sizeof(cmd_buff_t));

    //same loop as previous assignment
    //new implementations are commented
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
        

        // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff
        rc = build_cmd_buff(cmd_buff, cmd);                         //set return code to build_cmd_buff function
        if (rc == ERR_MEMORY){                                      //if there is a memory error
            break;                                                  //then break
        } else if (rc == ERR_CMD_OR_ARGS_TOO_BIG){                  //if there is an argument error
            break;                                                  //then break
        } else {
            // TODO IMPLEMENT if not built-in command, fork/exec as an external command
            // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
            if (strcmp(cmd->argv[0], "dragon") == 0){               //if command is dragon
                print_dragon();                                     //then print_dragon command
            } else if (strcmp(cmd->argv[0], "cd") == 0){            //else if command is cd
                if (cmd->argv[1] != NULL) {                         //then if the next argument is not null
                    chdir(cmd->argv[1]);                            //change directory to next argument
                }
            } else {                    
                pid_t pid = fork();                                 //create pid_t variable and fork
                if (pid == 0){                                      //if fork was ok
                    execvp(cmd->argv[0], cmd->argv);                //then execvp
                    rc = OK;                                        //set rc to ok
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
    free_cmd_buff(cmd);     //free cmd_buff
    free(cmd);              //free cmd

    return rc;              //return rc
}
