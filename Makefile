CC=gcc
buildDir=build

all: setup build/array.o build/music.o build/playlist.o build/main.o audio-player clean-build

setup:
	rm -rf $(buildDir)
	mkdir $(buildDir)

build/playlist.o:
	$(CC) -c -g playlist.c -o build/playlist.o

build/music.o:
	$(CC) -c -g music.c -o build/music.o


build/array.o:
	$(CC) -c -g libs/array.c -o build/array.o

build/main.o:
	$(CC) -c -g main.c -o build/main.o `pkg-config --cflags --libs gtk4 libvlc`

audio-player:
	$(CC) -o audio-player build/main.o build/playlist.o  build/music.o build/array.o `pkg-config --cflags --libs gtk4 libvlc`


clean-build:
	rm -rf $(buildDir)


clean:
	rm -rf build audio-player
run:
	./audio-player

install:
	cp audio-player /usr/bin