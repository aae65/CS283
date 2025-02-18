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

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff){
    if (cmd_line == NULL || cmd_buff == NULL){
        return ERR_MEMORY;
    }

    clear_cmd_buff(cmd_buff);

    cmd_buff->_cmd_buffer = strdup(cmd_line);
    if (cmd_buff->_cmd_buffer == NULL){
        return ERR_MEMORY;
    }

    char *token = strtok(cmd_buff->_cmd_buffer, " ");
    while (token != NULL && cmd_buff->argc < CMD_ARGV_MAX - 1){
        char *clean_token = token;
        char *dst = token;
        while (*token != '\0'){
            if (*token != '"' && *token != '\''){
                *dst++ = *token;
            }
            token++;
        }
        *dst = '\0';
        
        cmd_buff->argv[cmd_buff->argc++] = clean_token;
        token = strtok(NULL, " ");
    }
    cmd_buff->argv[cmd_buff->argc] = NULL;

    if (cmd_buff->argc >= CMD_ARGV_MAX - 1){
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff){
    if (cmd_buff == NULL){
        return ERR_MEMORY;
    }

    if (cmd_buff->_cmd_buffer != NULL){
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL;
    }

    for (int i = 0; i < CMD_ARGV_MAX; i++){
        cmd_buff->argv[i] = NULL;
    }
    cmd_buff->argc = 0;

    return OK;
}

int exec_local_cmd_loop()
{
    char cmd_buff[ARG_MAX];
    int rc = 0;
    cmd_buff_t *cmd = malloc(sizeof(cmd_buff_t));

    // TODO IMPLEMENT MAIN LOOP
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
        rc = build_cmd_buff(cmd_buff, cmd);
        if (rc == ERR_MEMORY){
            break;
        } else if (rc == ERR_CMD_OR_ARGS_TOO_BIG){
            break;
        } else {
            // TODO IMPLEMENT if not built-in command, fork/exec as an external command
            // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
            if (strcmp(cmd->argv[0], "dragon") == 0){
                print_dragon();
            } else if (strcmp(cmd->argv[0], "echo") == 0){
                for (int i = 1; i < cmd->argc; i++){
                    printf("%s", cmd->argv[i]);
                }
                printf("\n");
            } else if (strcmp(cmd->argv[0], "cd") == 0){
                if (cmd->argv[1] != NULL) {
                    chdir(cmd->argv[1]);
                }
            } else if (strcmp(cmd->argv[0], "pwd") == 0){
                //TODO
            }
        }
    
    }
    clear_cmd_buff(cmd);
    free(cmd);

    return rc;
}
