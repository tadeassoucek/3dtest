.DEFAULT := compile

CC=gcc
CFLAGS=-I src -Wall

compile:
	$(CC) main.c src/*.c -o bin/main $(CFLAGS)

video: compile
	bin/main
	ffmpeg -i "out/image%03d.ppm" -r 60 out/video.mp4
