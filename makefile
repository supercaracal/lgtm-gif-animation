CC = gcc

.PHONY: all debug clean

all: lgtm-gif-animation

debug: lgtm-gif-animation-debug

clean:
	rm -f lgtm-gif-animation lgtm-gif-animation-debug

lgtm-gif-animation: *.h *.c
	$(CC) -o $@ -Wall $^

lgtm-gif-animation-debug: *.h *.c
	$(CC) -o $@ -Wall -g -DDEBUG $^
