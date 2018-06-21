lgtm-gif-animation: $(wildcard *.h *.c)
	gcc -o $@ -Wall -g $^
