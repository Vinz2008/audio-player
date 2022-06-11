CC=gcc


all:
	rm -rf build
	mkdir build
	$(CC) -c -g libs/run_command.c -o build/run_command.o
	$(CC) -c -g libs/open_file_in_str.c -o build/open_file_in_str.o
	$(CC) -c -g main.c -o build/main.o `pkg-config --cflags --libs gtk4 libvlc`
	$(CC) -o audio-player build/main.o build/run_command.o build/open_file_in_str.o `pkg-config --cflags --libs gtk4 libvlc xkbcommon-x11 xcb`
	rm -rf build


clean:
	rm -rf build audio-player
run:
	./audio-player