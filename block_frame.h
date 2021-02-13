#ifndef BLOCK_FRAME_H_
#define BLOCK_FRAME_H_

#include "./type.h"

GIFBlockFrame *add_frame(GIFBlockFrame *);
void free_gif_frames(GIFBlockFrame *);

#endif  // BLOCK_FRAME_H_
