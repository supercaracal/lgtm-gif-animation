#ifndef BLOCK_H_
#define BLOCK_H_

#include <stdio.h>
#include "./constant.h"
#include "./type.h"
#include "./tool.h"
#include "./block_image.h"
#include "./block_ext.h"

void read_gif_blocks(struct gif_bytes *bytesp, struct gif_block_frame *framep, struct gif_block_ext_app *appp);
void write_gif_blocks(FILE *fp, const struct gif_block_frame *framep);

#endif // BLOCK_H_
