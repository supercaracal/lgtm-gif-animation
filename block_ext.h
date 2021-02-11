#ifndef BLOCK_EXT_H_
#define BLOCK_EXT_H_

#include "./type.h"

struct gif_block_frame *read_gif_block_ext(struct gif_bytes *, struct gif_block_frame *, struct gif_block_ext_app *);

#endif  // BLOCK_EXT_H_
