
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define SPACE_CHAR ' '

//prototypes for functions to handle required functionality

//#1 What is the purpose of providing prototypes for the functions in this code module?
    //The purpose of prototypes is to give anyone looking at the code a general idea of how the functions will work. It may also
    //serve as a good reminder for the coder if they need it.
void  usage(char *);
int   count_words(char *);
void  reverse_string(char *);
void  word_print(char *);


void usage(char *exename){
    printf("usage: %s [-h|c|r|w] \"string\" \n", exename);
    printf("\texample: %s -w \"hello class\" \n", exename);
}

//count_words algorithm
int count_words(char *str){
    //local variables
    int len = strlen(str);
    int wc;
    bool word_start = false; //create a boolean to indicate if you are at the start of a word and initialize to false

    for (int i = 0; i < len; i++){ //Loop over the length of the string
        //Get the current character aka str[i]
        if (!word_start){ //Is word_start state false?
            if (str[i] == SPACE_CHAR){ //Is the current character a SPACE_CHAR?
                continue;              //if YES, continue loop
            } else {                //if NO, we are at the start of a new word          
                wc++;               //increment wc
                word_start = true;  //set word_start to true
            }
        } else { //word_start is true
            if (str[i] == SPACE_CHAR){  //Is the current character a SPACE_CHAR?
                word_start = false;     //if YES we just ended a word, set word_start to false
        //if NO, its just a character in the current word so there is nothing more to do
            }
        }
    }
    return wc; //The current word count for the input string is in the wc variable so just 'return wc;'
}

//reverse_string() algorithm
//  1.  Initialize the start and end index variables
//      a.  end_idx is the length of str - 1.  We want to remove one
//          becuase at index str[len(str)] is the '\0' that we want
//          to preserve because we are using C strings.  That makes
//          the last real character in str as str[len(str)-1]
//      b.  start_idx is 0, thus str[0] is the first character in the
//          string.
//
//  2.  Loop while end_idx > start_idx
//      2a. swap the characters in str[start_idx] and str[end_idx]
//      2b. increment start_idx by 1
//      2c. decrement end_indx by 1
//
//  3. When the loop above terminates, the string should be reversed in place
void  reverse_string(char *str){
    // Suggested local variables
    int end_idx = strlen(str) - 1;  //length of string - 1
    int start_idx = 0;
    char tmp_char;

    while (end_idx > start_idx){
        tmp_char = str[start_idx];
        str[start_idx] = str[end_idx];
        str[end_idx] = tmp_char;

        start_idx++;
        end_idx--;
    }

}

//word_print() - algorithm
//
// Start by copying the code from count words.  Recall that that code counts
// individual words by incrementing wc when it encounters the first character 
// in a word.
// Now, at this point where we are incrementing wc we need to do a few more things
//      1. incrment wc, and set word_start to true like before
//      2. Now, set wlen to zero, as we will be counting characters in each word
//      3. Since we are starting a new word we can printf("%d. ", wc);
//
// If word_start is true, we are in an active word, so each time through the loop
// we would want to:
//      1. Check if the current character is not a SPACE_CHARACTER
//         a.  IF it is NOT A SPACE -> print the current character, increment wlen
//
//      2.  In the loop there are 2 conditions that indicate a current word is ending:
//          a. word_start is false and the current character is a SPACE_CHARACTER
//                  OR
//          b. the current loop index is the last character in the string (aka the
//             loop index is last_char_idx) 
//
//          IF either of these conditions are true:
//              - Print the word length for current word - printf(" (%d)\n", wlen);
//              - Set word_start to false
//              - Set wlen to 0 given we are starting a new word
//
// EXAMPLE OUTPUT
// ==============
// ./stringfun -w "C programming is fun"
// Word Print
// ----------
// 1. C (1)
// 2. programming (11)
// 3. is (2)
// 4. fun (3)
void  word_print(char *str){
    //suggested local variables
    int len = strlen(str);            //length of string - aka strlen(str);
    int last_char_idx = strlen(str) - 1;  //index of last char - strlen(str)-1;
    int wc = 0;         //counts words
    int wlen = 0;       //length of current word
    bool word_start = false;    //am I at the start of a new word

    for (int i = 0; i < len; i++){ //Loop over the length of the string
        //Get the current character aka str[i]
        if (!word_start){ //Is word_start state false?
            if (str[i] == SPACE_CHAR){ //Is the current character a SPACE_CHAR?
                continue;              //if YES, continue loop
            } else {                //if NO, we are at the start of a new word          
                wc++;               //increment wc
                word_start = true;  //set word_start to true
                wlen = 0;
                printf("%d. ", wc);
            }
        } else { //word_start is true
            if (str[i] != SPACE_CHAR){  //Is the current character not a SPACE_CHAR?
                printf("%c", str[i]);   //print the current character
                wlen++;                 //increment wlen
            }
            if ((!word_start && str[i] == SPACE_CHAR) || (i == last_char_idx)){
                printf(" (%d)\n", wlen);
                word_start = false;
                wlen = 0;
            }
        }
    }
}


int main(int argc, char *argv[]){
    char *input_string;     //holds the string provided by the user on cmd line
    char *opt_string;       //holds the option string in argv[1]
    char opt;               //used to capture user option from cmd line

    //THIS BLOCK OF CODE HANDLES PROCESSING COMMAND LINE ARGS
    if (argc < 2){
        usage(argv[0]);
        exit(1);
    }
    opt_string = argv[1];

    //note arv[2] should be -h -r -w or -c, thus the option is
    //the second character and a - is the first char
    if((opt_string[0] != '-') && (strlen(opt_string) != 2)){
        usage(argv[0]);
        exit(1);
    }

    opt = opt_string[1];   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //Finally the input string must be in argv[2]
    if (argc != 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2];
    //ALL ARGS PROCESSED - The string you are working with is
    //is the third arg or in arv[2]
    
    switch (opt){
        case 'c':
            int wc = 0;         //variable for the word count

            //#2. Call count_words, return of the result should go into the wc variable
            wc = count_words(input_string);
            printf("Word Count: %d\n", wc);
            break;
        case 'r':
            //#3. Call reverse string using input_string input string should be reversed
            reverse_string(input_string);
            printf("Reversed string: %s\n", input_string);

            //#4.  The algorithm provided in the directions state we simply return after swapping all 
            //     characters because the string is reversed in place.  Briefly explain why the string 
            //     is reversed in place - place in a comment.
                //The string is reversed in place for a few reasons. One of them being memory tracking. Adding several other variables
                //to swap then replace the original string can take upe a lot of memory, thus can impact runtime. Another reason is
                //that input_string is a global variable. Nothing has to be returned and the variable can be used without needing to be
                //manually updated.
            break;
        case 'w':
            printf("Word Print\n----------\n");

            //#5. Call word_print, output should be printed by that function
            word_print(input_string);
            break;

        //#6. What is the purpose of the default option here?
            //The purpose of the default option is to prevent errors. Specifically infinite loops. If the 
            //code does not exit as soon as an error occurs, more problems are able to occur.
        default:
            usage(argv[0]);
            printf("Invalid option %c provided, exiting!\n", opt);
            exit(1);
    }
    //#7. Why did we place a break statement on each case option, and did not place one on default?
    //    What would happen if we forgot the break statement?
            //Adding a break statement for each case option stops the code from running continuously 
            //and falling through to the next segment. There was no statement on default because
            //the code exits. Forgetting a break statement will cause the code to continue to run,
            //preventing the user from seeing the output (if any) from the code.
}
