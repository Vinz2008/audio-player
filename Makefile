CC=gcc

CFLAGS=-c -g -Wall -O2 $(shell pkg-config --cflags gtk4 libvlc)
LDFLAGS=$(shell pkg-config --libs gtk4 libvlc)

SRCDIR=src

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))


all: audio-player

audio-player: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -o $@ $^

clean-build:
	rm -f ./*.o ./libs/*.o


clean: clean-build
	rm -f audio-player
run:
	./audio-player

install:
	cp audio-player /usr/bin