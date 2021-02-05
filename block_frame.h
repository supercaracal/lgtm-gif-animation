#ifndef BLOCK_FRAME_H_
#define BLOCK_FRAME_H_

#include "./type.h"
#include "./tool.h"

struct gif_block_frame *add_frame(struct gif_block_frame *prev_framep);
void dealloc_gif_frames(struct gif_block_frame *frame);

#endif  // BLOCK_FRAME_H_
