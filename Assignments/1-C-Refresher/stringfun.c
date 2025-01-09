#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    //#4:  Implement the setup buff as per the directions
    int str_length = 0;
    for (int i = 0; i < user_str; i++){ //while string is not equal to the null terminator
        if (*(user_str + i) != '\t' && *(user_str + i) != ' ' && *(user_str + i) != '\n'){ //if char is a non-white space character
            *(buff + str_length) = *(user_str + i); //then copy char to buffer
        } else {
            if (*(user_str + i - 1) != '\t' && *(user_str + i - 1) != ' ' && *(user_str + i - 1) != '\n'){ //if previous char added was NOT a white-space character
                *(buff + str_length) = ' '; //then copy space to buffer
            }
        }
        str_length++;
    }
    if (str_length > len){ //if string is greater than buffer size
        return -1; //then return string too large code
    }
    
    while (str_length < BUFFER_SZ){ //while length of buffer is not equal to 50
        *(buff + str_length) = '.'; //add '.' to buffer
        user_str++; 
    }

    return str_length; //return size of user string 
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    return 0; //IMPLEMENT
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //#1. WHY IS THIS SAFE, aka what if argv[1] does not exist?
    //*     This is safe because it catches the input before running into trouble.
    //*     argv[0] is the filename (executable) that is being run and argv[1] is being
    //*     used as the parameter for the function. argc is the number of arguments, argv
    //*     are the arguments themselves. If argv[1] did not exist, the program would automatically
    //*     exit, as argc would be less than 2.             
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //#2 Document the purpose of the if statement below
    //*     The purpose of the if statement is to determine if the number of arguments
    //*     is less than 3. If it is, it prints the intended usage for the program
    //*     then exits with code 1. The reason this is here is to prevent errors from 
    //*     happening within the code, as there has to be 3 arguments for the code to run properly.     
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //#3. Allocate space for the buffer using malloc and
    //    handle error if malloc fails by exiting with a 
    //    return code of 99
    *buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if(buff == NULL){
        fprintf(stderr, "Memory allocation failed");
        exit(99);
    }


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE