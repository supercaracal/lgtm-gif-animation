#ifndef BLOCK_H_
#define BLOCK_H_

#include <stdio.h>
#include "./type.h"

void read_gif_blocks(BinData *, GIFBlockFrame *, GIFBlockExtApp *);
void write_gif_blocks(FILE *, const GIFBlockFrame *);

#endif  // BLOCK_H_
