.DEFAULT := compile
.PHONY := compile clean

CC=gcc
CFLAGS=-I . -Wall -O3
BINARY=bin/3dtest

compile:
	$(CC) main.c src/*.c -o $(BINARY) $(CFLAGS)

video: compile clean
	$(BINARY)
	ffmpeg -i "out/image%03d.ppm" -r 60 out/video.mp4 -y
	mpv --loop out/video.mp4

clean:
	rm -rf out/*
