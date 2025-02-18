#include <stdio.h>

// EXTRA CREDIT - print the drexel dragon from the readme.md
extern void print_dragon(){
    FILE *file = fopen("dragon.txt", "r");          //open file
    char line[256];                                 //set line buff
    while (fgets(line, sizeof(line), file)) {       //while not at EOF
        printf("%s", line);                         //print line
    }
    printf("\n");                                   //print newline to preserve spacing for rest of code
    fclose(file);                                   //close file
}