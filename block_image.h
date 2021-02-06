#ifndef BLOCK_IMAGE_H_
#define BLOCK_IMAGE_H_

#include "./type.h"
#include "./tool.h"
#include "./block_frame.h"

struct gif_block_frame *read_gif_block_img(struct gif_bytes *, struct gif_block_frame *);

#endif  // BLOCK_IMAGE_H_
