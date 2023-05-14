#include "file.h"
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "array.h"

const char* findFileExtension(const char* filename){
    for (int i = 0; i < strlen(filename); i++){
        if (filename[i] == '.'){
            char* str = malloc(sizeof(char) * 5);
            for (int j = i; j < strlen(filename); j++){
                strncat(str, &filename[j], 1);
            }
            return str;
        }
    }
    printf("file extension not found\n");
    return "";
}

void findFileRecursively(const char* path, char** returnedArray){
    int i = 0;
    DIR *dir;
    struct dirent *dp;
    char * file_name;
    dir = opendir(path);
    while ((dp=readdir(dir)) != NULL) {   
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){
        file_name = dp->d_name; 
        append_to_array(file_name, returnedArray, i);
        i++;
        printf("The File Name :-  \"%s\"\n",file_name);
        
        }
    }
    closedir(dir);
}