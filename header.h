#ifndef HEADER_H_
#define HEADER_H_

#include <stdio.h>
#include <string.h>
#include "./constant.h"
#include "./type.h"
#include "./tool.h"

void
read_gif_header(struct gif_bytes  *bytesp,
                struct gif_header *hp);

void
write_gif_header(FILE                    *fp,
                 const struct gif_header *hp);

void
dealloc_gif_header(struct gif_header *hp);

#endif // HEADER_H_
