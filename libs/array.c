#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** append_to_array(char* thing, char** array){
    char** tempArray = (char**)malloc(sizeof(array) + sizeof(thing));
    int templength = 0;
    int iTemp = 0; 
    /*for (int i3 = 0; i3 < 1; i3++){
        printf("array[%d] : %s\n",i3, array[i3]);
    }*/
    printf("TEST\n");
    /*while (array[iTemp]){
        printf("array[iTemp] : %s\n", array[iTemp]);
        templength++;
        printf("templength : %d\n", templength);
        iTemp++;
    }*/
    printf("test\n");
    templength = sizeof(array)/sizeof(array[0]);
    printf("templength : %d\n", templength);
    int end = 0;
    for (int i = 0; i < templength - 1; i++){
        tempArray[i] = (char*)malloc(strlen(array[i]) * sizeof(char));
        strcpy(tempArray[i], array[i]);
        end = i;
        printf("end : %i\n", end);
    }
    printf("end last : %i\n", end);
    tempArray[end + 1] = (char*)malloc(strlen(thing) * sizeof(char));
    printf("TEST2\n");
    strcpy(tempArray[end + 1], thing);
    printf("TEST3\n");
    int iTemp2 = 0;
    printf("TEST4\n");
    /*while (iTemp2 < templength + 1){
        printf("TEST!\n");
        printf("tempArray[%d] : %s\n", iTemp2, tempArray[iTemp2]);
        iTemp2++;
    }*/
    printf("function end\n");
    return tempArray;
}


char** remove_from_array_last(char** array){
    char** tempArray = malloc(sizeof(array) - sizeof(char*));
    int templength;
    int iTemp = 0; 
    while (array[iTemp]){
        templength++;
        iTemp++;
    }
    for (int i = 0; i < templength - 1; i++){
        strcpy(tempArray[i], array[i]);
    }
    return tempArray;
}

char** remove_from_array(char* thing, char** array){
    int i;
    int i2 = 0;
    int pos;
    char** arrayCopied;
    char** tempArray = malloc(sizeof(array) - sizeof(thing));
    int templength;
    int iTemp = 0;
    while (array[iTemp]){
        templength++;
        iTemp++;
    }
    for (i = 0; i < templength; i++){
        strcpy(arrayCopied[i], array[i]);
    }
    for (i = 0; i < templength; i++){
        if (arrayCopied[i] == thing){
            pos = i;
        }
    }
    for (i = 0; i < templength; i++){
        if (i != pos){
            strcpy(tempArray[i], arrayCopied[i2]);
        }
        i2++;
    }
    return tempArray;
}