#ifndef BLOCK_FRAME_H_
#define BLOCK_FRAME_H_

#include "./type.h"

struct gif_block_frame *add_frame(struct gif_block_frame *);
void free_gif_frames(struct gif_block_frame *);

#endif  // BLOCK_FRAME_H_
