CC := gcc
CFLAGS += -Wall
src_headers := *.c *.h

.PHONY: all debug clean

all: lgtm-gif-animation
debug: lgtm-gif-animation-debug
clean:
	rm -f lgtm-gif-animation lgtm-gif-animation-debug

define build-bin
  $(strip $(LINK.c)) $(OUTPUT_OPTION) $^
endef

lgtm-gif-animation: $(src_headers)
	$(build-bin)

lgtm-gif-animation-debug: CFLAGS += -g
lgtm-gif-animation-debug: CPPFLAGS += -DDEBUG
lgtm-gif-animation-debug: $(src_headers)
	$(build-bin)
