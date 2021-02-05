SHELL := /bin/bash
CC    ?= gcc

CFLAGS += -std=c11 -D_POSIX_C_SOURCE=200809
CFLAGS += -Wall -Wextra -Wpedantic -Wundef

SRCS := block block_ext block_ext_app block_ext_comment block_ext_gp_ctrl block_ext_plain_text block_frame block_image data header lgtm tool
OBJS := $(addsuffix .o,$(SRCS))

define link
	$(strip $(LINK.o)) $^ $(LOADLIBES) $(LDLIBS) -o $@
endef

build: lgtm-gif-animation

lgtm-gif-animation: CFLAGS += -O2
lgtm-gif-animation: main.o $(OBJS)
	$(call link)

debug: lgtm-gif-animation-debug

lgtm-gif-animation-debug: CFLAGS += -g
lgtm-gif-animation-debug: CPPFLAGS += -DDEBUG
lgtm-gif-animation-debug: main.o $(OBJS)
	$(call link)

lint:
	@type cpplint
	@cpplint *.h *.c

clean:
	@rm -f lgtm-gif-animation lgtm-gif-animation-debug *.o

.PHONY: build debug lint clean
