MAKEFLAGS += --warn-undefined-variables
SHELL     := /bin/bash -euo pipefail
CC        ?= gcc
CFLAGS    += -std=c11
CFLAGS    += -D_POSIX_C_SOURCE=200809
CFLAGS    += -Wall
CFLAGS    += -Wextra
CFLAGS    += -Wpedantic
CFLAGS    += -Wundef
SRCS      += block
SRCS      += block_ext
SRCS      += block_ext_app
SRCS      += block_ext_comment
SRCS      += block_ext_gp_ctrl
SRCS      += block_ext_plain_text
SRCS      += block_frame
SRCS      += block_image
SRCS      += header
SRCS      += lgtm
SRCS      += tool
OBJS      := $(addsuffix .o,$(SRCS))

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
