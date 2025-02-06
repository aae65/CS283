#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "dshlib.h"
 
/*
* Implement your main function by building a loop that prompts the
* user for input.  Use the SH_PROMPT constant from dshlib.h and then
* use fgets to accept user input.  Since we want fgets to also handle
* end of file so we can run this headless for testing we need to check
* the return code of fgets.  I have provided an example below of how
* to do this assuming you are storing user input inside of the cmd_buff
* variable.
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
*
*   Expected output:
*
*      CMD_OK_HEADER      if the command parses properly. You will
*                         follow this by the command details
*
*      CMD_WARN_NO_CMD    if the user entered a blank command
*      CMD_ERR_PIPE_LIMIT if the user entered too many commands using
*                         the pipe feature, e.g., cmd1 | cmd2 | ... |
*
*  See the provided test cases for output expectations.
*/
int main() {
    char cmd_buff[ARG_MAX];                                     //allocate max argument size to buffer
    int rc = 0;
    command_list_t *clist = malloc(sizeof(command_list_t));     //allocate size of struct to list
 
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
    exit(EXIT_SUCCESS);         //exit on success
}