#include "run_command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int run_command(char* command, char* output){
    int ch;
    int i = 0;
    char* tempstr = malloc(1000 * sizeof(char));
    FILE* temp = popen(command, "r");
    while((ch=fgetc(temp)) != EOF) {
        //putchar(ch);
        tempstr[i] = ch;
        i++;
    }
    pclose(temp);
    printf("tempstr: %s\n", tempstr);
    memset(tempstr,0,sizeof(tempstr));
    return 0;
}