

#ifndef __musicAudioPlayer__

#define __musicAudioPlayer__
typedef struct {
    char* path;
    char* name;
} Music;

Music createMusic(char* name, char* path);


#endif