#include "music.h"
#include <string.h>


Music createMusic(char* name, char* path){
    Music tempMusic;
    tempMusic.path = path;
    tempMusic.name = name;
    return tempMusic;
}