#ifndef HEADER_H_
#define HEADER_H_

#include <stdio.h>
#include "./type.h"

void read_gif_header(BinData *, GIFHeader *);
void write_gif_header(FILE *, const GIFHeader *);
void free_gif_header(GIFHeader *);

#endif  // HEADER_H_
