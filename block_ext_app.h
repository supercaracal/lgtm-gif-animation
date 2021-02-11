#ifndef BLOCK_EXT_APP_H_
#define BLOCK_EXT_APP_H_

#include <stdio.h>
#include "./type.h"

void read_gif_block_ext_app(struct gif_bytes *, struct gif_block_ext_app *);
void write_gif_ext_app(FILE *, const struct gif_block_ext_app *);

#endif  // BLOCK_EXT_APP_H_
