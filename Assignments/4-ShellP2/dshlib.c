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
int exec_local_cmd_loop()
{
    char cmd_buff[ARG_MAX];                                     //allocate max argument size to buffer
    int rc = 0;
    command_list_t *clist = malloc(sizeof(command_list_t));     //allocate size of struct to list

    // TODO IMPLEMENT MAIN LOOP
    while(1){
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
            printf("\n");
            break;
        }
        //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 
        //IMPLEMENT THE REST OF THE REQUIREMENTS
        if (strcmp(cmd_buff, EXIT_CMD) == 0){               //if user inputs exit
            break;                                          //then break
        }

        if (strcmp(cmd_buff, "") == 0){                     //if user enters nothing
            printf(CMD_WARN_NO_CMD);                        //then print no command error
            continue;                                       //continue
        }
 
        rc = build_cmd_list(cmd_buff, clist);                                                       //set return code to build list function
        if(rc == ERR_TOO_MANY_COMMANDS){                                                            //if there are too many commands
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);                                                    //then print too many commands
        } else if (rc == ERR_CMD_OR_ARGS_TOO_BIG) {                                                 //if too many args and commands
            printf(CMD_ERR_PIPE_LIMIT, 8);                                                          //then print pipe error
        } else {                                                                                    //else
            printf(CMD_OK_HEADER, clist->num);                                                      //print header string
            for (int j = 0, k = 1; j < clist->num; j++, k++){                                       //iterate through list with two vars, one iterating and one counting
                if (clist->commands[j].args[0] != '\0'){                                            //if there are args
                    printf("<%d> %s[%s]\n", k, clist->commands[j].exe, clist->commands[j].args);    //print them
                } else {                                                                            //else
                    printf("<%d> %s\n", k, clist->commands[j].exe);                                 //print the command
                }
            }
        }
    }
    free(clist);                //free list

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

    return OK;
}