lgtm-gif-animation: $(wildcard *.h *.c)
	gcc -o $@ -Wall $^

debug: $(wildcard *.h *.c)
	gcc -o $@ -Wall -g $^
