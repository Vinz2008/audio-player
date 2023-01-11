#include "playlist.h"
#include <stdlib.h>

void initPlaylist(Playlist* playlist, size_t initialSize){
    playlist->musicList = malloc(initialSize * sizeof(Music));
    playlist->used = 0;
    playlist->size = initialSize;
}

void addMusicToPlaylist(Playlist* playlist, Music music){
    if (playlist->used == playlist->size){
        playlist->size *=2;
        playlist->musicList = realloc(playlist->musicList, playlist->size * sizeof(Music));
    }
    playlist->musicList[playlist->used++] = music;
}

void emptyPlaylist(Playlist* playlist){
    free(playlist->musicList);
    playlist->musicList = NULL;
    playlist->used = playlist->size = 0;
}