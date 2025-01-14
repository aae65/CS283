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
int reverse_string(char *, int);
int word_print(char *, int);
int search_and_replace(char *, char *, char *);


int setup_buff(char *buff, char *user_str, int len){
    //#4:  Implement the setup buff as per the directions
    int str_length = 0;
    for (int i = 0; *(user_str + i) != '\0'; i++){ //while string is not equal to the null terminator
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
    len = str_length;
    
    while (str_length < BUFFER_SZ){ //while length of buffer is not equal to 50
        *(buff + str_length) = '.'; //add '.' to buffer
        str_length++;
    }

    return len; //return size of user string 
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
    int wc = 0;         //initalize word count

    if (str_len > len){         //if string length is greater than buffer length
        return -2;                  //then return with -2
    }

    for(int i = 0; i < str_len; i++){                               //for loop
        if (*(buff + i) == ' ' && i > 0 && *(buff + i - 1) != ' '){     //if buffer char is not space and i is not 0 and previous buffer char is not space
            wc++;                                                           //then increase word count
        }
    }

    if (str_len > 0 && *(buff + str_len - 1) != ' '){               //if string length is greater than 0 and char is not space
        wc++;                                                           //then increase word count
    }

    return wc;                                                      //return word count
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
int reverse_string(char *str, int str_len){
    char temp;

    for (int i = 0; i < str_len / 2; i++) {
        temp = *(str + i);
        *(str + i) = *(str + str_len - i - 1);
        *(str + str_len - i - 1) = temp;
    }

    return 0;

}

int word_print(char *str, int str_len){
    int wlen = 0;                           //initilize word length
    int wc = 0;                             //initalize word count

    printf("Word Print\n----------\n");             //print beginning

    while (*str != '\0'){                           //while string is not null terminated
        while (*str == ' '){                            //while string is space
            str++;                                          //move to next spot in string
        }

        if (*str != '\0'){                              //if string is not null terminated
            wc++;                                           //then increase word count
            printf("%d. ", wc);                             //print number

            wlen = 0;                                       //set word length to 0
            while (*str != ' ' && *str != '\0'){            //while string is not space and not null terminated
                putchar(*str);                                  //put character to stdout
                str++;                                          //move to next char
                wlen++;                                         //increase word length
            }

            printf(" (%d)\n", wlen);                        //print # of chars
        }
        
        if(wlen > str_len){                             //if word length is greater than string length
            return -2;                                      //return with -2
        }
    }
    return 0;                                       
}

int search_and_replace(char *user_str, char *search, char *replace){
    printf("Replacing %c with %c in %c.\n", *user_str, *replace, *search);  //placeholder to prevent warning from compiler
    printf("Not implemented!\n");                                           //
    return -1;                                                              //return with -1
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //#1.   WHY IS THIS SAFE, aka what if argv[1] does not exist?
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

    //#2    Document the purpose of the if statement below
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
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if(buff == NULL){
        fprintf(stderr, "Memory allocation failed\n");
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
                printf("Error counting words, rc = %d\n", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //#5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            rc = reverse_string(input_string, user_str_len);
            if (rc < 0){
                printf("Error reversing words, rc = %d\n", rc);
                exit(2);
            }
            printf("Reversed string: %s\n", input_string);
            break;

        case 'w':
            rc = word_print(input_string, user_str_len);
            if (rc < 0){
                printf("Error printing words, rc = %d\n", rc);
                exit(2);
            }
            break;

        case 'x':
            if (argc != 5){
                printf("Not enough arguments\n");
                exit(2);
            }
            rc = search_and_replace(input_string, argv[2], argv[3]);
            if (rc < 0){
                exit(2);
            }
            break;

        default:
            usage(argv[0]);
            exit(1);
    }

    //#6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//    #7    Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//*         It is important to include the length and the buffer  
//*         because, when dealing with reallocation of memory,  
//*         the length is not always going to remain the same.
//*         For example, a string needing to be motified may
//*         be larger than the provided allocation. So, the
//*         length would have to be reallocated. Assuming the
//*         string would be used for more than function, the 
//*         length should always be provided.