#ifndef HEADER_H_
#define HEADER_H_

#include <stdio.h>
#include "./type.h"

void read_gif_header(struct gif_bytes *, struct gif_header *);
void write_gif_header(FILE *, const struct gif_header *);
void free_gif_header(struct gif_header *);

#endif  // HEADER_H_
