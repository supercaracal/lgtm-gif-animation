#ifndef BLOCK_H_
#define BLOCK_H_

#include <stdio.h>
#include "./type.h"

void read_gif_blocks(struct gif_bytes *, struct gif_block_frame *, struct gif_block_ext_app *);
void write_gif_blocks(FILE *, const struct gif_block_frame *);

#endif  // BLOCK_H_
