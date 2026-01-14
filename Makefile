.DEFAULT := compile

CC=gcc
CFLAGS=-I src -Wall -O3

compile:
	$(CC) main.c src/*.c -o bin/main $(CFLAGS)

video: compile clean
	bin/main
	ffmpeg -i "out/image%03d.ppm" -r 60 out/video.mp4 -y
	mpv --loop out/video.mp4

terminal:
	$(CC) main_terminal.c src/*.c -o bin/terminal $(CFLAGS)

clean:
	rm -rf out/*
