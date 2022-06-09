#include <stdio.h>
#include <stdlib.h>

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