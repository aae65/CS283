#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

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

//build cmd buff function
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff){
    cmd_buff->argc = 0;                                 //reset arg variable
    char *token = strtokq(cmd_line);                    //create blank token

    while (token != NULL){                              //while token is not empty
        cmd_buff->argv[cmd_buff->argc++] = token;       //add token to cmd_buff struct
        token = strtokq(NULL);                          //call strtokq to get new token
    }

    cmd_buff->argv[cmd_buff->argc] = NULL;              //set last pointer to null

    if (cmd_buff->argc >= CMD_ARGV_MAX){                //if argc is over limit
        return ERR_CMD_OR_ARGS_TOO_BIG;                 //then return error
    }

    return OK;
}

//strtok quote function
char *strtokq(char *str){                                     
    char *token;                       
    static char *next;

    if (str != NULL){                   //if token is not null
        token = str;                    //set to input string
    } else {                            //else
        token = next;                   //set to static var to keep
    }
    
    if (*token == '\0'){                                        //check if null terminated
        return NULL;
    }    
    while (isspace(*token)){                                    //while token is space
        token++;                                                //continue through token
    }
    if (*token == '\0'){
        return NULL;
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

    next = token;                                               //get rest of token

    return stripped_token;
}

//build cmd list function
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

        cmd_buff_t *cmd = &clist->commands[clist->num++];        //create struct for commands and args

        int rc = build_cmd_buff(token, cmd);
        if (rc != OK){
            return rc;
        }

        token = strtok_r(rest, PIPE_STRING, &rest);             //get new token
    }

    return OK;                                                  //return ok
}

int exec_local_cmd_loop()
{
    char cmd_buff[ARG_MAX];
    int rc = 0;
    command_list_t *clist = malloc(sizeof(command_list_t));

    while (1){
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
            printf("\n");
            break;
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        if (strcmp(cmd_buff, EXIT_CMD) == 0){
            rc = EXIT_SUCCESS;
            break;
        }

        if (strcmp(cmd_buff, "") == 0){
            printf(CMD_WARN_NO_CMD);
            continue;
        }
        
        rc = build_cmd_list(cmd_buff, clist);
        if (rc != OK){                                      //if there is a memory error
            return rc;                                                  //then break
        } else if (clist->num == 1){
            if (strcmp(clist->commands[0].argv[0], "dragon") == 0){               //if command is dragon
                print_dragon();                                     //then print_dragon command
            } else if (strcmp(clist->commands[0].argv[0], "cd") == 0){            //else if command is cd
                if (clist->commands[0].argv[1] != NULL) {                         //then if the next argument is not null
                    chdir(clist->commands[0].argv[1]);                            //change directory to next argument
                }
            } else {
                pid_t pid = fork();
                if (pid == 0){
                    execvp(clist->commands[0].argv[0], clist->commands[0].argv);
                } else {
                    int status;
                    waitpid(pid, &status, 0);
                }
            }
        } else {
            int num_cmds = clist->num;
            int pipe_fds[2];
            int prev_pipe = -1;

            for (int i = 0; i < clist->num; i++){                   //iterate over clist
                if (i < num_cmds - 1){                              //if i is less than commands
                    pipe(pipe_fds);                                 //then pipe
                }
                                  
                pid_t pid = fork();                                 //create pid_t variable and fork
                if (pid == 0){                                      //if in child process
                    if (i == 0){                                    //then if i is first command
                        dup2(pipe_fds[1], STDOUT_FILENO);           //then begin pipe
                    } else if (i == num_cmds - 1){                  //else if i is last command
                        dup2(prev_pipe, STDIN_FILENO);              //then finish pipe
                    } else {                                        //else
                        dup2(prev_pipe, STDIN_FILENO);              //continue piping
                        dup2(pipe_fds[1], STDOUT_FILENO);
                    }

                    close(pipe_fds[0]);                             //close pipes
                    close(pipe_fds[1]);
                    if (prev_pipe != -1){                           //if prev is not empty
                        close(prev_pipe);                           //then close it
                    }

                    execvp(clist->commands[i].argv[0], clist->commands[i].argv);  
                    exit(1);
                } 

                if (prev_pipe != -1){                               //check again for open pipe
                    close(pipe_fds[prev_pipe]);
                }
                prev_pipe = pipe_fds[0];
                close(pipe_fds[1]);
            }

            for (int i = 0; i < num_cmds; i++){                 //wait for child processes to finish
                wait(NULL);
            }
        }
    }

    free(clist);

    return rc;
}