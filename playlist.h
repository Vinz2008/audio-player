#include <stddef.h>
#include "music.h"

typedef struct {
    Music* musicList;
    size_t used;
    size_t size;
} Playlist;



void initPlaylist(Playlist* playlist, size_t initalSize);

void addMusicToPlaylist(Playlist* playlist, Music music);

void emptyPlaylist(Playlist* playlist);