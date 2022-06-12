#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** append_to_array(char* thing, char** array){
    char** tempArray = malloc(sizeof(array) + sizeof(thing));
    int templength;
    int iTemp = 0; 
    while (array[iTemp]){
        templength++;
        iTemp++;
    }
    int end;
    for (int i = 0; i < templength; i++){
        strcpy(tempArray[i], array[i]);
        end = i;
    }
    strcpy(tempArray[end + 1], thing);
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