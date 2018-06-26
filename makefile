CC := gcc
CFLAGS += -Wall
all_sources := *.c *.h

.PHONY: all debug clean

all: lgtm-gif-animation
debug: lgtm-gif-animation-debug
clean:
	rm -f lgtm-gif-animation lgtm-gif-animation-debug

define build-bin
  $(strip $(LINK.c)) $(OUTPUT_OPTION) $^
endef

lgtm-gif-animation: $(all_sources)
	$(build-bin)

lgtm-gif-animation-debug: CFLAGS += -g
lgtm-gif-animation-debug: CPPFLAGS += -DDEBUG
lgtm-gif-animation-debug: $(all_sources)
	$(build-bin)
