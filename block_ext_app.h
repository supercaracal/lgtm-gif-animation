#ifndef BLOCK_EXT_APP_H_
#define BLOCK_EXT_APP_H_

#include <stdio.h>
#include "./type.h"
#include "./tool.h"

void read_gif_block_ext_app(struct gif_bytes *bytesp, struct gif_block_ext_app *appp);
void write_gif_ext_app(FILE *fp, const struct gif_block_ext_app *appp);

#endif  // BLOCK_EXT_APP_H_
